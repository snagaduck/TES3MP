#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketActorAuthority.hpp"

using namespace mwmp;

PacketActorAuthority::PacketActorAuthority(mwmp::NetworkManager *network) : ActorPacket(network)
{
    packetID = ID_ACTOR_AUTHORITY;
}

void PacketActorAuthority::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    BasePacket::Packet(newBitstream, send);

    RW(actorList->cell.mData, send, true);
    RW(actorList->cell.mName, send, true);
}
