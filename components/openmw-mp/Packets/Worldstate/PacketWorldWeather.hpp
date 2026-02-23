#ifndef OPENMW_PACKETWORLDWEATHER_HPP
#define OPENMW_PACKETWORLDWEATHER_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>

namespace mwmp
{
    class PacketWorldWeather : public WorldstatePacket
    {
    public:
        PacketWorldWeather(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETWORLDWEATHER_HPP
