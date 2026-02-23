#include "PacketPlayerResurrect.hpp"
#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/TimedLog.hpp>

using namespace mwmp;

PacketPlayerResurrect::PacketPlayerResurrect(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_RESURRECT;
}

void PacketPlayerResurrect::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->resurrectType, send);
}
