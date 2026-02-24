#ifndef OPENMW_NETWORKING_HPP
#define OPENMW_NETWORKING_HPP

#include <components/openmw-mp/Controllers/SystemPacketController.hpp>
#include <components/openmw-mp/Controllers/PlayerPacketController.hpp>
#include <components/openmw-mp/Controllers/ActorPacketController.hpp>
#include <components/openmw-mp/Controllers/ObjectPacketController.hpp>
#include <components/openmw-mp/Controllers/WorldstatePacketController.hpp>
#include <components/openmw-mp/Net/PlayerId.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>
#include <components/openmw-mp/Packets/PacketPreInit.hpp>
#include "GNSNetworkManager.hpp"
#include "Player.hpp"

class MasterClient;
namespace  mwmp
{
    class Networking
    {
    public:
        Networking(uint16_t port, const std::string& address, unsigned int maxConnections);
        ~Networking();

        void newPlayer(mwmp::PlayerId guid);
        void disconnectPlayer(mwmp::PlayerId guid);
        void kickPlayer(mwmp::PlayerId guid, bool sendNotification = true);

        void banAddress(const char *ipAddress);
        void unbanAddress(const char *ipAddress);
        std::string getSystemAddress(mwmp::PlayerId guid);

        GNSNetworkManager *getGNSManager() const;

        void processSystemPacket(mwmp::ReceivedPacket &rp);
        void processPlayerPacket(mwmp::ReceivedPacket &rp);
        void processActorPacket(mwmp::ReceivedPacket &rp);
        void processObjectPacket(mwmp::ReceivedPacket &rp);
        void processWorldstatePacket(mwmp::ReceivedPacket &rp);
        void update(mwmp::ReceivedPacket &rp);

        unsigned short numberOfConnections() const;
        unsigned int maxConnections() const;
        int getAvgPing(mwmp::PlayerId pid) const;
        unsigned short getPort() const;

        int mainLoop();

        void stopServer(int code);

        SystemPacketController *getSystemPacketController() const;
        PlayerPacketController *getPlayerPacketController() const;
        ActorPacketController *getActorPacketController() const;
        ObjectPacketController *getObjectPacketController() const;
        WorldstatePacketController *getWorldstatePacketController() const;

        BaseActorList *getReceivedActorList();
        BaseObjectList *getReceivedObjectList();
        BaseWorldstate *getReceivedWorldstate();

        int getCurrentMpNum();
        void setCurrentMpNum(int value);
        int incrementMpNum();

        bool getDataFileEnforcementState();
        void setDataFileEnforcementState(bool state);

        bool getScriptErrorIgnoringState();
        void setScriptErrorIgnoringState(bool state);

        MasterClient *getMasterClient();
        void InitQuery(std::string queryAddr, unsigned short queryPort);
        void setServerPassword(std::string passw) noexcept;
        bool isPassworded() const;

        static const Networking &get();
        static Networking *getPtr();

        void postInit();

        PacketPreInit::PluginContainer &getSamples();
    private:
        bool preInit(mwmp::ReceivedPacket &rp, mwmp::PlayerId pid);
        std::string serverPassword;
        static Networking *sThis;

        GNSNetworkManager *gnsManager;
        mwmp::NetBuffer bsOut;
        TPlayers *players;
        MasterClient *mclient;

        BaseSystem baseSystem;
        BaseActorList baseActorList;
        BaseObjectList baseObjectList;
        BaseWorldstate baseWorldstate;

        SystemPacketController *systemPacketController;
        PlayerPacketController *playerPacketController;
        ActorPacketController *actorPacketController;
        ObjectPacketController *objectPacketController;
        WorldstatePacketController *worldstatePacketController;

        bool running;
        int exitCode;
        PacketPreInit::PluginContainer samples;
    };
}


#endif //OPENMW_NETWORKING_HPP
