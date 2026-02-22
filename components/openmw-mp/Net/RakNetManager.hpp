#ifndef OPENMW_RAKNETMANAGER_HPP
#define OPENMW_RAKNETMANAGER_HPP

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <PacketPriority.h>

#include "NetworkManager.hpp"
#include "PlayerId.hpp"

namespace mwmp
{
    // Temporary bridge: implements NetworkManager on top of RakNet::RakPeerInterface.
    // Used in Segments 3-5 so every segment independently builds while we migrate types.
    // Removed in Segment 7 once both sides are on GNS.
    class RakNetManager : public NetworkManager
    {
    public:
        explicit RakNetManager(RakNet::RakPeerInterface* peer);

        // --- NetworkManager interface ---
        void Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel) override;
        void Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel) override;
        void CloseConnection(PlayerId pid, bool sendNotification) override;
        std::string GetAddress(PlayerId pid) const override;
        PlayerId GetMyId() const override;

        // --- RakNet-bridge helpers (called by Networking.cpp) ---

        // Register a new RakNet GUID <-> PlayerId mapping.
        // Call when a new connection arrives.
        PlayerId RegisterGuid(RakNet::RakNetGUID guid);

        // Remove the mapping when a connection closes.
        void UnregisterGuid(RakNet::RakNetGUID guid);

        // Convert in both directions.
        PlayerId      ToPlayerId(RakNet::RakNetGUID guid) const;
        RakNet::RakNetGUID ToGuid(PlayerId pid) const;

        bool HasGuid(RakNet::RakNetGUID guid) const;
        bool HasPlayerId(PlayerId pid) const;

        RakNet::RakPeerInterface* GetPeer() const { return peer; }

    private:
        RakNet::RakPeerInterface* peer;

        mutable std::mutex mapMutex;
        std::unordered_map<uint64_t, PlayerId>          guidToId;  // guid.g -> PlayerId
        std::unordered_map<PlayerId, RakNet::RakNetGUID> idToGuid;

        std::atomic<PlayerId> nextId{1}; // starts at 1; 0 == INVALID_PLAYER_ID
    };
}

#endif // OPENMW_RAKNETMANAGER_HPP
