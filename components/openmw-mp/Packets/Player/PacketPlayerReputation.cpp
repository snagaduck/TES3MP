#include "PacketPlayerReputation.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerReputation::PacketPlayerReputation(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_REPUTATION;
}

void PacketPlayerReputation::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->npcStats.mReputation, send);
}
