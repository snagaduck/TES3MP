#ifndef OPENMW_PACKETCONTAINER_HPP
#define OPENMW_PACKETCONTAINER_HPP

#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>

namespace mwmp
{
    class PacketContainer : public ObjectPacket
    {
    public:
        PacketContainer(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETCONTAINER_HPP
