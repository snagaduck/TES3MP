#ifndef OPENMW_PACKETOBJECTRESTOCK_HPP
#define OPENMW_PACKETOBJECTRESTOCK_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectRestock : public ObjectPacket
    {
    public:
        PacketObjectRestock(mwmp::NetworkManager *network);
    };
}

#endif //OPENMW_PACKETOBJECTRESTOCK_HPP
