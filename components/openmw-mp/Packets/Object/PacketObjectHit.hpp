#ifndef OPENMW_PACKETOBJECTHIT_HPP
#define OPENMW_PACKETOBJECTHIT_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectHit : public ObjectPacket
    {
    public:
        PacketObjectHit(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETOBJECTHIT_HPP
