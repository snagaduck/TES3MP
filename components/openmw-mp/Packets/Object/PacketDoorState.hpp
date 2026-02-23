#ifndef OPENMW_PACKETDOORSTATE_HPP
#define OPENMW_PACKETDOORSTATE_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketDoorState : public ObjectPacket
    {
    public:
        PacketDoorState(mwmp::NetworkManager *network);

        virtual void Object(BaseObject &baseObject, bool send);
    };
}

#endif //OPENMW_PACKETDOORSTATE_HPP
