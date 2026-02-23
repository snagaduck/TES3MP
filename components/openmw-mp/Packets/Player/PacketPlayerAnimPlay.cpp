#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketPlayerAnimPlay.hpp"

mwmp::PacketPlayerAnimPlay::PacketPlayerAnimPlay(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_ANIM_PLAY;
}

void mwmp::PacketPlayerAnimPlay::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->animation.groupname, send);
    RW(player->animation.mode, send);
    RW(player->animation.count, send);
    RW(player->animation.persist, send);
}
