#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketWorldRegionAuthority.hpp"

mwmp::PacketWorldRegionAuthority::PacketWorldRegionAuthority(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_WORLD_REGION_AUTHORITY;
}

void mwmp::PacketWorldRegionAuthority::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->authorityRegion, send, true);
}
