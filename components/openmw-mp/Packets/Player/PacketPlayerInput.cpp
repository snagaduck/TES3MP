#include "PacketPlayerInput.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>

using namespace mwmp;

PacketPlayerInput::PacketPlayerInput(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_INPUT;
}

void PacketPlayerInput::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    // Placeholder
}
