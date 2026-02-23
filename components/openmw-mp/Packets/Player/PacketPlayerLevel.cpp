#include "PacketPlayerLevel.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerLevel::PacketPlayerLevel(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_LEVEL;
}

void PacketPlayerLevel::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->creatureStats.mLevel, send);

    RW(player->npcStats.mLevelProgress, send);
}
