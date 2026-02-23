#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketPlayerAnimFlags.hpp"

mwmp::PacketPlayerAnimFlags::PacketPlayerAnimFlags(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_ANIM_FLAGS;
}

void mwmp::PacketPlayerAnimFlags::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->movementFlags, send);
    RW(player->drawState, send);
    RW(player->isJumping, send);
    RW(player->isFlying, send);
    RW(player->hasTcl, send);
}
