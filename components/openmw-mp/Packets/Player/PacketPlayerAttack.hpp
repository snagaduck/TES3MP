#ifndef OPENMW_PACKETPLAYERATTACK_HPP
#define OPENMW_PACKETPLAYERATTACK_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerAttack : public PlayerPacket
    {
    public:
        PacketPlayerAttack(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERATTACK_HPP
