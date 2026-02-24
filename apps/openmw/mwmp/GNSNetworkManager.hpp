#ifndef OPENMW_CLIENT_GNSNETWORKMANAGER_HPP
#define OPENMW_CLIENT_GNSNETWORKMANAGER_HPP

#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>
#include <steam/steamnetworkingsockets.h>
#include <atomic>
#include <mutex>
#include <string>
#include <vector>

namespace mwmp
{
    // Client-side GNS NetworkManager: connects to one server.
    class GNSNetworkManager : public NetworkManager
    {
    public:
        GNSNetworkManager();
        ~GNSNetworkManager() override;

        // Connect to server; returns true on success.
        bool Connect(const std::string& host, uint16_t port);
        void Disconnect();

        // Poll messages from the server; strips 9-byte header.
        std::vector<ReceivedPacket> PollMessages();

        // State events accumulated by callback
        enum class EventType { Connected, Disconnected, Failed };
        struct Event { EventType type; };
        std::vector<Event> PollEvents();

        bool isConnected() const { return serverConn != k_HSteamNetConnection_Invalid; }

        // NetworkManager interface
        void Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel) override;
        void Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel) override;
        void CloseConnection(PlayerId pid, bool notify) override;
        std::string GetAddress(PlayerId pid) const override;
        PlayerId GetMyId() const override;

        // Server's assigned PlayerId for this client
        PlayerId getServerPlayerId() const { return serverPlayerId; }

    private:
        static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
        static GNSNetworkManager* sInstance;

        ISteamNetworkingSockets* sockets;
        HSteamNetConnection serverConn;
        PlayerId serverPlayerId;

        mutable std::mutex eventMutex;
        std::vector<Event> pendingEvents;
    };
}

#endif // OPENMW_CLIENT_GNSNETWORKMANAGER_HPP
