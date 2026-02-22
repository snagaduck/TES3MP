#include "RakNetManager.hpp"

#include <RakPeer.h>

using namespace mwmp;

RakNetManager::RakNetManager(RakNet::RakPeerInterface* peer)
    : peer(peer)
{}

// ---------------------------------------------------------------------------
// NetworkManager interface
// ---------------------------------------------------------------------------

void RakNetManager::Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel)
{
    RakNet::RakNetGUID guid = ToGuid(pid);
    if (guid == RakNet::UNASSIGNED_CRABNET_GUID)
        return;

    PacketReliability rel = reliable ? RELIABLE_ORDERED : UNRELIABLE;
    PacketPriority    pri = HIGH_PRIORITY;
    peer->Send(reinterpret_cast<const char*>(buf.GetData()),
               static_cast<int>(buf.GetSize()),
               pri, rel, static_cast<char>(channel),
               RakNet::AddressOrGUID(guid), false);
}

void RakNetManager::Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel)
{
    PacketReliability rel = reliable ? RELIABLE_ORDERED : UNRELIABLE;
    PacketPriority    pri = HIGH_PRIORITY;

    RakNet::RakNetGUID excludeGuid = RakNet::UNASSIGNED_CRABNET_GUID;
    if (exclude != INVALID_PLAYER_ID)
        excludeGuid = ToGuid(exclude);

    // RakNet broadcast: pass the exclude GUID with broadcast=true
    peer->Send(reinterpret_cast<const char*>(buf.GetData()),
               static_cast<int>(buf.GetSize()),
               pri, rel, static_cast<char>(channel),
               RakNet::AddressOrGUID(excludeGuid), true);
}

void RakNetManager::CloseConnection(PlayerId pid, bool sendNotification)
{
    RakNet::RakNetGUID guid = ToGuid(pid);
    if (guid != RakNet::UNASSIGNED_CRABNET_GUID)
        peer->CloseConnection(guid, sendNotification);
}

std::string RakNetManager::GetAddress(PlayerId pid) const
{
    RakNet::RakNetGUID guid = ToGuid(pid);
    if (guid == RakNet::UNASSIGNED_CRABNET_GUID)
        return "";
    return peer->GetSystemAddressFromGuid(guid).ToString();
}

PlayerId RakNetManager::GetMyId() const
{
    return INVALID_PLAYER_ID; // server has no single "my ID"
}

// ---------------------------------------------------------------------------
// Bridge helpers
// ---------------------------------------------------------------------------

PlayerId RakNetManager::RegisterGuid(RakNet::RakNetGUID guid)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = guidToId.find(guid.g);
    if (it != guidToId.end())
        return it->second;

    PlayerId id = nextId++;
    guidToId[guid.g] = id;
    idToGuid[id]     = guid;
    return id;
}

void RakNetManager::UnregisterGuid(RakNet::RakNetGUID guid)
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = guidToId.find(guid.g);
    if (it != guidToId.end())
    {
        idToGuid.erase(it->second);
        guidToId.erase(it);
    }
}

PlayerId RakNetManager::ToPlayerId(RakNet::RakNetGUID guid) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = guidToId.find(guid.g);
    return (it != guidToId.end()) ? it->second : INVALID_PLAYER_ID;
}

RakNet::RakNetGUID RakNetManager::ToGuid(PlayerId pid) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    auto it = idToGuid.find(pid);
    return (it != idToGuid.end()) ? it->second : RakNet::UNASSIGNED_CRABNET_GUID;
}

bool RakNetManager::HasGuid(RakNet::RakNetGUID guid) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return guidToId.count(guid.g) > 0;
}

bool RakNetManager::HasPlayerId(PlayerId pid) const
{
    std::lock_guard<std::mutex> lock(mapMutex);
    return idToGuid.count(pid) > 0;
}
