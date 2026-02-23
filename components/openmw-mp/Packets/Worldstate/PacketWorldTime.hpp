#ifndef OPENMW_PACKETWORLDTIME_HPP
#define OPENMW_PACKETWORLDTIME_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>

namespace mwmp
{
    class PacketWorldTime : public WorldstatePacket
    {
    public:
        PacketWorldTime(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETWORLDTIME_HPP
