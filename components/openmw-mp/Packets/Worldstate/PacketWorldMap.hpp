#ifndef OPENMW_PACKETWORLDMAP_HPP
#define OPENMW_PACKETWORLDMAP_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>

namespace mwmp
{
    class PacketWorldMap : public WorldstatePacket
    {
    public:
        PacketWorldMap(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETWORLDMAP_HPP
