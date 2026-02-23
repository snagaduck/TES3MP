#ifndef OPENMW_PACKETOBJECTSTATE_HPP
#define OPENMW_PACKETOBJECTSTATE_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketObjectState : public ObjectPacket
    {
    public:
        PacketObjectState(mwmp::NetworkManager *network);

        virtual void Object(BaseObject &baseObject, bool send);
    };
}

#endif //OPENMW_PACKETOBJECTSTATE_HPP
