#include "GNSNetworkManager.hpp"
#include <steam/isteamnetworkingutils.h>
#include <components/openmw-mp/TimedLog.hpp>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <utility>

using namespace mwmp;

GNSNetworkManager* GNSNetworkManager::sInstance = nullptr;

GNSNetworkManager::GNSNetworkManager()
    : sockets(nullptr)
    , serverConn(k_HSteamNetConnection_Invalid)
    , serverPlayerId(1) // one server, assign constant id
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
    Disconnect();
    GameNetworkingSockets_Kill();
    sInstance = nullptr;
}

bool GNSNetworkManager::Connect(const std::string& host, uint16_t port)
{
    SteamNetworkingIPAddr addr;
    addr.Clear();
    std::string addrStr = host + ":" + std::to_string(port);
    if (!addr.ParseString(addrStr.c_str()))
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "GNS client: invalid address %s", addrStr.c_str());
        return false;
    }

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
               reinterpret_cast<void*>(&GNSNetworkManager::OnConnectionStatusChanged));

    serverConn = sockets->ConnectByIPAddress(addr, 1, &opt);
    if (serverConn == k_HSteamNetConnection_Invalid)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "GNS client: ConnectByIPAddress failed for %s", addrStr.c_str());
        return false;
    }

    // Block until connected or failed
    while (true)
    {
        sockets->RunCallbacks();

        auto events = PollEvents();
        for (auto& ev : events)
        {
            if (ev.type == EventType::Connected)
                return true;
            if (ev.type == EventType::Failed || ev.type == EventType::Disconnected)
                return false;
        }

        // Brief sleep to avoid busy-waiting
        SteamNetworkingSockets()->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void GNSNetworkManager::Disconnect()
{
    if (serverConn != k_HSteamNetConnection_Invalid)
    {
        sockets->CloseConnection(serverConn, 0, "Client disconnect", true);
        serverConn = k_HSteamNetConnection_Invalid;
    }
}

std::vector<ReceivedPacket> GNSNetworkManager::PollMessages()
{
    std::vector<ReceivedPacket> result;
    sockets->RunCallbacks();

    if (serverConn == k_HSteamNetConnection_Invalid)
        return result;

    constexpr int kBatchSize = 64;
    SteamNetworkingMessage_t* msgs[kBatchSize];
    int count = sockets->ReceiveMessagesOnConnection(serverConn, msgs, kBatchSize);

    for (int i = 0; i < count; i++)
    {
        SteamNetworkingMessage_t* msg = msgs[i];
        const uint8_t* data = static_cast<const uint8_t*>(msg->m_pData);
        uint32_t len = static_cast<uint32_t>(msg->m_cbSize);

        if (len >= 1)
        {
            const size_t hdrLen = 1 + sizeof(PlayerId);
            ReceivedPacket rp;
            rp.packetId = data[0];
            rp.sender = serverPlayerId;
            rp.senderAddress = GetAddress(serverPlayerId);
            if (len > hdrLen)
                rp.data = NetBuffer(data + hdrLen, len - hdrLen);
            result.push_back(std::move(rp));
        }
        msg->Release();
    }

    return result;
}

std::vector<GNSNetworkManager::Event> GNSNetworkManager::PollEvents()
{
    std::lock_guard<std::mutex> lock(eventMutex);
    return std::exchange(pendingEvents, {});
}

void GNSNetworkManager::Send(PlayerId /*pid*/, const NetBuffer& buf, bool reliable, int /*channel*/)
{
    if (serverConn == k_HSteamNetConnection_Invalid)
        return;
    int flags = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;
    sockets->SendMessageToConnection(serverConn, buf.GetData(),
        static_cast<uint32>(buf.GetSize()), flags, nullptr);
}

void GNSNetworkManager::Broadcast(const NetBuffer& buf, PlayerId /*exclude*/, bool reliable, int /*channel*/)
{
    // Client only ever sends to server
    Send(serverPlayerId, buf, reliable, 0);
}

void GNSNetworkManager::CloseConnection(PlayerId /*pid*/, bool notify)
{
    Disconnect();
    (void)notify;
}

std::string GNSNetworkManager::GetAddress(PlayerId /*pid*/) const
{
    if (serverConn == k_HSteamNetConnection_Invalid)
        return "";
    SteamNetConnectionInfo_t info;
    if (!sockets->GetConnectionInfo(serverConn, &info))
        return "";
    char buf[SteamNetworkingIPAddr::k_cchMaxString];
    info.m_addrRemote.ToString(buf, sizeof(buf), true);
    return buf;
}

PlayerId GNSNetworkManager::GetMyId() const
{
    return INVALID_PLAYER_ID;
}

void GNSNetworkManager::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
{
    if (!sInstance)
        return;

    GNSNetworkManager& self = *sInstance;
    ESteamNetworkingConnectionState newState = info->m_info.m_eState;

    if (newState == k_ESteamNetworkingConnectionState_Connected)
    {
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "GNS client: connected to server");
        std::lock_guard<std::mutex> lock(self.eventMutex);
        self.pendingEvents.push_back({EventType::Connected});
        return;
    }

    if (newState == k_ESteamNetworkingConnectionState_ClosedByPeer
        || newState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
    {
        ESteamNetworkingConnectionState oldState = info->m_eOldState;
        bool wasConnecting = (oldState == k_ESteamNetworkingConnectionState_Connecting
                           || oldState == k_ESteamNetworkingConnectionState_FindingRoute);

        if (wasConnecting)
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "GNS client: connection attempt failed");
        else
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "GNS client: disconnected from server");

        // Clean up the connection handle
        self.sockets->CloseConnection(info->m_hConn, 0, nullptr, false);
        self.serverConn = k_HSteamNetConnection_Invalid;

        std::lock_guard<std::mutex> lock(self.eventMutex);
        self.pendingEvents.push_back({wasConnecting ? EventType::Failed : EventType::Disconnected});
    }
}
