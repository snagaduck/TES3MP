#ifndef OPENMW_NETWORKMANAGER_HPP
#define OPENMW_NETWORKMANAGER_HPP

#include <string>

#include "PlayerId.hpp"
#include "NetBuffer.hpp"

namespace mwmp
{
    // Abstract transport interface — replaces direct RakNet::RakPeerInterface usage.
    // Concrete implementations: RakNetManager (bridge), GNSNetworkManager (final).
    class NetworkManager
    {
    public:
        virtual ~NetworkManager() = default;

        // Send to a single peer.
        virtual void Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel) = 0;

        // Send to all peers except `exclude` (use INVALID_PLAYER_ID to send to all).
        virtual void Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel) = 0;

        // Close a connection.
        virtual void CloseConnection(PlayerId pid, bool sendNotification) = 0;

        // Get printable address string for a peer.
        virtual std::string GetAddress(PlayerId pid) const = 0;

        // The local player/server ID (server-side: typically unused / returns INVALID_PLAYER_ID).
        virtual PlayerId GetMyId() const = 0;
    };
}

#endif // OPENMW_NETWORKMANAGER_HPP
