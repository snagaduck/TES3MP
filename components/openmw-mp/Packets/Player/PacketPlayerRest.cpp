#include "PacketPlayerRest.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerRest::PacketPlayerRest(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_REST;
}

void PacketPlayerRest::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    // Placeholder to be filled in later
}
