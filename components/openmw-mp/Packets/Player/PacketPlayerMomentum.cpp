#include "PacketPlayerMomentum.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerMomentum::PacketPlayerMomentum(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_MOMENTUM;
}

void PacketPlayerMomentum::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);
    
    RW(player->momentum.pos, send, true);
}
