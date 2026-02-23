#include "PacketPlayerPosition.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerPosition::PacketPlayerPosition(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_POSITION;
}

void PacketPlayerPosition::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->position, send, 1);
    RW(player->direction, send, 1);
}
