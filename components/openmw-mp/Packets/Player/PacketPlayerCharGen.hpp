#ifndef OPENMW_PACKETPLAYERCHARGEN_HPP
#define OPENMW_PACKETPLAYERCHARGEN_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerCharGen : public PlayerPacket
    {
    public:
        PacketPlayerCharGen(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERCHARGEN_HPP
