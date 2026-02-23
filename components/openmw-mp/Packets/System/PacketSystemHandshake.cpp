#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketSystemHandshake.hpp"

using namespace mwmp;

PacketSystemHandshake::PacketSystemHandshake(mwmp::NetworkManager *network) : SystemPacket(network)
{
    packetID = ID_SYSTEM_HANDSHAKE;
    orderChannel = CHANNEL_SYSTEM;
}

void PacketSystemHandshake::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    SystemPacket::Packet(newBitstream, send);

    if (!RW(system->playerName, send, true, maxNameLength) ||
        !RW(system->serverPassword, send, true, maxPasswordLength))
    {
        packetValid = false;
        return;
    }
}
