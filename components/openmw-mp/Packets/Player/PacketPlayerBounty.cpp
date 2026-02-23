#include "PacketPlayerBounty.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerBounty::PacketPlayerBounty(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_BOUNTY;
}

void PacketPlayerBounty::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->npcStats.mBounty, send);
}
