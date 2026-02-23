#ifndef OPENMW_PACKETPLAYERMOMENTUM_HPP
#define OPENMW_PACKETPLAYERMOMENTUM_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerMomentum : public PlayerPacket
    {
    public:
        PacketPlayerMomentum(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERMOMENTUM_HPP
