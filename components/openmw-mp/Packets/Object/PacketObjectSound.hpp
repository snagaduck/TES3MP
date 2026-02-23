#ifndef OPENMW_PACKETOBJECTSOUND_HPP
#define OPENMW_PACKETOBJECTSOUND_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectSound : public ObjectPacket
    {
    public:
        PacketObjectSound(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETOBJECTSOUND_HPP
