#ifndef OPENMW_GNSNETWORKMANAGER_HPP
#define OPENMW_GNSNETWORKMANAGER_HPP

#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>
#include <steam/steamnetworkingsockets.h>
#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mwmp
{
    class GNSNetworkManager : public NetworkManager
    {
    public:
        explicit GNSNetworkManager(unsigned int maxConnections);
        ~GNSNetworkManager() override;

        bool Init(uint16_t port, const std::string& address);

        // Poll game messages; caller wraps results into ReceivedPackets
        std::vector<ReceivedPacket> PollMessages();

        // Events accumulated by the static callback
        struct ConnectionEvent
        {
            enum Type { Connected, Disconnected } type;
            PlayerId pid;
            std::string address;
        };
        std::vector<ConnectionEvent> PollConnectionEvents();

        // NetworkManager interface
        void Send(PlayerId pid, const NetBuffer& buf, bool reliable, int channel) override;
        void Broadcast(const NetBuffer& buf, PlayerId exclude, bool reliable, int channel) override;
        void CloseConnection(PlayerId pid, bool notify) override;
        std::string GetAddress(PlayerId pid) const override;
        PlayerId GetMyId() const override;

        // Server info helpers
        unsigned int numberOfConnections() const;
        unsigned int maxConnections() const;
        int getAvgPing(PlayerId pid) const;
        uint16_t getPort() const;

        void banAddress(const std::string& ip);
        void unbanAddress(const std::string& ip);

    private:
        static void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);
        static GNSNetworkManager* sInstance;

        ISteamNetworkingSockets* sockets;
        HSteamListenSocket listenSocket;
        HSteamNetPollGroup pollGroup;

        unsigned int maxConns;
        uint16_t boundPort;

        mutable std::mutex mapMutex;
        std::atomic<PlayerId> nextId{1};
        std::unordered_map<HSteamNetConnection, PlayerId> connToId;
        std::unordered_map<PlayerId, HSteamNetConnection> idToConn;
        std::unordered_map<PlayerId, std::string> idToAddr;

        std::mutex eventMutex;
        std::vector<ConnectionEvent> pendingEvents;

        std::unordered_set<std::string> bannedIPs;
    };
}

#endif // OPENMW_GNSNETWORKMANAGER_HPP
