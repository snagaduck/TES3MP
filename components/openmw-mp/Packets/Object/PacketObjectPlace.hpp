#ifndef OPENMW_PACKETOBJECTPLACE_HPP
#define OPENMW_PACKETOBJECTPLACE_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectPlace : public ObjectPacket
    {
    public:
        PacketObjectPlace(mwmp::NetworkManager *network);

        virtual void Object(BaseObject &baseObject, bool send);
    };
}

#endif //OPENMW_PACKETOBJECTPLACE_HPP
