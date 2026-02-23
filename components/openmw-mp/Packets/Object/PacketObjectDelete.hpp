#ifndef OPENMW_PACKETOBJECTDELETE_HPP
#define OPENMW_PACKETOBJECTDELETE_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectDelete : public ObjectPacket
    {
    public:
        PacketObjectDelete(mwmp::NetworkManager *network);
    };
}

#endif //OPENMW_PACKETOBJECTDELETE_HPP
