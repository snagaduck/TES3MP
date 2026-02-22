#include "PacketPlayerReputation.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerReputation::PacketPlayerReputation(RakNet::RakPeerInterface *peer) : PlayerPacket(peer)
{
    packetID = ID_PLAYER_REPUTATION;
}

void PacketPlayerReputation::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->npcStats.mReputation, send);
}
