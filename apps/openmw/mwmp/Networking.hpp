#ifndef OPENMW_NETWORKING_HPP
#define OPENMW_NETWORKING_HPP

#include <string>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>

#include <components/openmw-mp/NetworkMessages.hpp>

#include <components/openmw-mp/Controllers/SystemPacketController.hpp>
#include <components/openmw-mp/Controllers/PlayerPacketController.hpp>
#include <components/openmw-mp/Controllers/ActorPacketController.hpp>
#include <components/openmw-mp/Controllers/ObjectPacketController.hpp>
#include <components/openmw-mp/Controllers/WorldstatePacketController.hpp>

#include <components/files/collections.hpp>

#include "GNSNetworkManager.hpp"
#include "LocalSystem.hpp"
#include "ActorList.hpp"
#include "ObjectList.hpp"
#include "Worldstate.hpp"

namespace mwmp
{
    class LocalPlayer;

    class Networking
    {
    public:
        Networking();
        ~Networking();
        void connect(const std::string& ip, unsigned short port, std::vector<std::string> &content, Files::Collections &collections);
        void update();

        SystemPacket *getSystemPacket(unsigned char id);
        PlayerPacket *getPlayerPacket(unsigned char id);
        ActorPacket *getActorPacket(unsigned char id);
        ObjectPacket *getObjectPacket(unsigned char id);
        WorldstatePacket *getWorldstatePacket(unsigned char id);

        bool isConnected();

        LocalSystem *getLocalSystem();
        LocalPlayer *getLocalPlayer();
        ActorList *getActorList();
        ObjectList *getObjectList();
        Worldstate *getWorldstate();

    private:
        bool connected;
        // gnsManager must be declared before the controller value-members so that
        // C++ initializes it first in the member-initializer list.
        GNSNetworkManager gnsManager;
        mwmp::PlayerId serverPlayerId;
        mwmp::NetBuffer bsOut;

        SystemPacketController systemPacketController;
        PlayerPacketController playerPacketController;
        ActorPacketController actorPacketController;
        ObjectPacketController objectPacketController;
        WorldstatePacketController worldstatePacketController;

        ActorList actorList;
        ObjectList objectList;
        Worldstate worldstate;

        void receiveMessage(mwmp::ReceivedPacket &rp);

        void preInit(std::vector<std::string> &content, Files::Collections &collections);
    };
}


#endif //OPENMW_NETWORKING_HPP
