#ifndef OPENMW_PACKETSTATSDYNAMIC_HPP
#define OPENMW_PACKETSTATSDYNAMIC_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerStatsDynamic : public PlayerPacket
    {
    public:
        PacketPlayerStatsDynamic(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETSTATSDYNAMIC_HPP
