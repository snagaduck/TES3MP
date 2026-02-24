#include "GNSNetworkManager.hpp"
#include <steam/isteamnetworkingutils.h>
#include <components/openmw-mp/TimedLog.hpp>
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <utility>

using namespace mwmp;

GNSNetworkManager* GNSNetworkManager::sInstance = nullptr;

GNSNetworkManager::GNSNetworkManager(unsigned int maxConnections)
    : sockets(nullptr)
    , listenSocket(k_HSteamListenSocket_Invalid)
    , pollGroup(k_HSteamNetPollGroup_Invalid)
    , maxConns(maxConnections)
    , boundPort(0)
{
    assert(!sInstance);
    sInstance = this;

    SteamNetworkingErrMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg))
        throw std::runtime_error(std::string("GameNetworkingSockets_Init failed: ") + errMsg);

    sockets = SteamNetworkingSockets();
}

GNSNetworkManager::~GNSNetworkManager()
{
    if (listenSocket != k_HSteamListenSocket_Invalid)
        sockets->CloseListenSocket(listenSocket);
    if (pollGroup != k_HSteamNetPollGroup_Invalid)
        sockets->DestroyPollGroup(pollGroup);

    GameNetworkingSockets_Kill();
    sInstance = nullptr;
}

bool GNSNetworkManager::Init(uint16_t port, const std::string& address)
{
    SteamNetworkingIPAddr localAddr;
    localAddr.Clear();
    if (!address.empty() && address != "0.0.0.0")
        localAddr.ParseString(address.c_str());
    localAddr.m_port = port;

    // Register connection status callback via config option on the listen socket
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
               reinterpret_cast<void*>(&GNSNetworkManager::OnConnectionStatusChanged));

    listenSocket = sockets->CreateListenSocketIP(localAddr, 1, &opt);
    if (listenSocket == k_HSteamListenSocket_Invalid)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "GNS: Failed to create listen socket on port %d", port);
        return false;
    }

    pollGroup = sockets->CreatePollGroup();
    if (pollGroup == k_HSteamNetPollGroup_Invalid)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "GNS: Failed to create poll group");
        return false;
    }

    boundPort = port;
    LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "GNS: Listening on port %d", port);
    return true;
}

std::vector<ReceivedPacket> GNSNetworkManager::PollMessages()
{
    std::vector<ReceivedPacket> result;

    // Dispatch pending connection events first
    sockets->RunCallbacks();

    constexpr int kBatchSize = 64;
    SteamNetworkingMessage_t* msgs[kBatchSize];
    int count = sockets->ReceiveMessagesOnPollGroup(pollGroup, msgs, kBatchSize);

    for (int i = 0; i < count; i++)
    {
        SteamNetworkingMessage_t* msg = msgs[i];
        const uint8_t* data = static_cast<const uint8_t*>(msg->m_pData);
        uint32_t len = static_cast<uint32_t>(msg->m_cbSize);

        if (len < 1)
        {
            msg->Release();
            continue;
        }

        PlayerId pid;
        {
            std::lock_guard<std::mutex> lock(mapMutex);
            auto it = connToId.find(msg->m_conn);
            if (it == connToId.end())
            {
                msg->Release();
                continue;
            }
            pid = it->second;
        }

        const size_t hdrLen = 1 + sizeof(PlayerId);
        ReceivedPacket rp;
        rp.packetId = data[0];
        rp.sender = pid;
        rp.senderAddress = GetAddress(pid);
        if (len > hdrLen)
            rp.data = NetBuffer(data + hdrLen, len - hdrLen);

        result.push_back(std::move(rp));
        msg->Release();
    }

    return result;
}

std::vector<GNSNetworkManager::ConnectionEvent> GNSNetworkManager::PollConnectionEvents()
{
    std::lock_guard<std::mutex> lock(eventMutex);
    return std::exchange(pendingEvents, {});
}

void GNSNetworkManager::Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel)
{
    HSteamNetConnection conn;
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = idToConn.find(pid);
        if (it == idToConn.end())
            return;
        conn = it->second;
    }
    int flags = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;
    sockets->SendMessageToConnection(conn, buf.GetData(), static_cast<uint32>(buf.GetSize()), flags, nullptr);
}

void GNSNetworkManager::Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel)
{
    int flags = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;
    std::lock_guard<std::mutex> lock(mapMutex);
    for (auto& [conn, pid] : connToId)
    {
        if (pid == exclude)
            continue;
        sockets->SendMessageToConnection(conn, buf.GetData(), static_cast<uint32>(buf.GetSize()), flags, nullptr);
    }
}

void GNSNetworkManager::CloseConnection(PlayerId pid, bool notify)
{
    HSteamNetConnection conn;
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = idToConn.find(pid);
        if (it == idToConn.end())
            return;
        conn = it->second;
    }
    sockets->CloseConnection(conn, 0, nullptr, notify);
}

std::string GNSNetworkManager::GetAddress(PlayerId pid) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = idToAddr.find(pid);
    return it != idToAddr.end() ? it->second : "";
}

PlayerId GNSNetworkManager::GetMyId() const
{
    return INVALID_PLAYER_ID; // server has no self-identity in this model
}

unsigned int GNSNetworkManager::numberOfConnections() const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return static_cast<unsigned int>(connToId.size());
}

unsigned int GNSNetworkManager::maxConnections() const
{
    return maxConns;
}

int GNSNetworkManager::getAvgPing(PlayerId pid) const
{
    HSteamNetConnection conn;
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = idToConn.find(pid);
        if (it == idToConn.end())
            return -1;
        conn = it->second;
    }
    SteamNetConnectionRealTimeStatus_t status;
    if (sockets->GetConnectionRealTimeStatus(conn, &status, 0, nullptr) == k_EResultOK)
        return status.m_nPing;
    return -1;
}

uint16_t GNSNetworkManager::getPort() const
{
    return boundPort;
}

void GNSNetworkManager::banAddress(const std::string& ip)
{
    bannedIPs.insert(ip);
}

void GNSNetworkManager::unbanAddress(const std::string& ip)
{
    bannedIPs.erase(ip);
}

void GNSNetworkManager::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
{
    if (!sInstance)
        return;

    GNSNetworkManager& self = *sInstance;
    HSteamNetConnection hConn = info->m_hConn;
    ESteamNetworkingConnectionState newState = info->m_info.m_eState;
    ESteamNetworkingConnectionState oldState = info->m_eOldState;

    if (newState == k_ESteamNetworkingConnectionState_Connecting
        && oldState == k_ESteamNetworkingConnectionState_None)
    {
        // Incoming connection request — check ban list
        char addrBuf[SteamNetworkingIPAddr::k_cchMaxString];
        info->m_info.m_addrRemote.ToString(addrBuf, sizeof(addrBuf), false); // false = no port
        std::string ip(addrBuf);

        if (self.bannedIPs.count(ip))
        {
            self.sockets->CloseConnection(hConn, 0, "Banned", false);
            return;
        }

        if (self.numberOfConnections() >= self.maxConns)
        {
            self.sockets->CloseConnection(hConn, 0, "Server full", false);
            return;
        }

        self.sockets->AcceptConnection(hConn);
        self.sockets->SetConnectionPollGroup(hConn, self.pollGroup);
        return;
    }

    if (newState == k_ESteamNetworkingConnectionState_Connected
        && oldState == k_ESteamNetworkingConnectionState_Connecting)
    {
        // Connection fully established — assign PlayerId
        char addrBuf[SteamNetworkingIPAddr::k_cchMaxString];
        info->m_info.m_addrRemote.ToString(addrBuf, sizeof(addrBuf), true); // true = include port
        std::string addr(addrBuf);

        PlayerId pid = self.nextId.fetch_add(1);
        {
            std::lock_guard<std::mutex> lock(self.mapMutex);
            self.connToId[hConn] = pid;
            self.idToConn[pid] = hConn;
            self.idToAddr[pid] = addr;
        }

        {
            std::lock_guard<std::mutex> lock(self.eventMutex);
            self.pendingEvents.push_back({ConnectionEvent::Connected, pid, addr});
        }

        LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "GNS: Client connected from %s (pid=%llu)", addr.c_str(), (unsigned long long)pid);
        return;
    }

    if (newState == k_ESteamNetworkingConnectionState_ClosedByPeer
        || newState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
    {
        PlayerId pid = INVALID_PLAYER_ID;
        std::string addr;
        {
            std::lock_guard<std::mutex> lock(self.mapMutex);
            auto it = self.connToId.find(hConn);
            if (it != self.connToId.end())
            {
                pid = it->second;
                auto addrIt = self.idToAddr.find(pid);
                if (addrIt != self.idToAddr.end())
                    addr = addrIt->second;
                self.idToConn.erase(pid);
                self.idToAddr.erase(pid);
                self.connToId.erase(it);
            }
        }

        if (pid != INVALID_PLAYER_ID)
        {
            std::lock_guard<std::mutex> lock(self.eventMutex);
            self.pendingEvents.push_back({ConnectionEvent::Disconnected, pid, addr});
        }

        self.sockets->CloseConnection(hConn, 0, nullptr, false);

        if (newState == k_ESteamNetworkingConnectionState_ClosedByPeer)
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "GNS: Client %s disconnected", addr.c_str());
        else
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "GNS: Client %s lost connection", addr.c_str());
    }
}
