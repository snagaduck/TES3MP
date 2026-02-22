#ifndef OPENMW_PACKETWORLDCOLLISIONOVERRIDE_HPP
#define OPENMW_PACKETWORLDCOLLISIONOVERRIDE_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>

namespace mwmp
{
    class PacketWorldCollisionOverride : public WorldstatePacket
    {
    public:
        PacketWorldCollisionOverride(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETWORLDCOLLISIONOVERRIDE_HPP
