#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketWorldRegionAuthority.hpp"

mwmp::PacketWorldRegionAuthority::PacketWorldRegionAuthority(mwmp::NetworkManager *network) : WorldstatePacket(network)
{
    packetID = ID_WORLD_REGION_AUTHORITY;
    // Make sure the priority is lower than PlayerCellChange's, so it doesn't get sent before it
}

void mwmp::PacketWorldRegionAuthority::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    WorldstatePacket::Packet(newBitstream, send);

    RW(worldstate->authorityRegion, send, true);
}
