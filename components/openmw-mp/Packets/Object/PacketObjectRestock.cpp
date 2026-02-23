#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketObjectRestock.hpp"

using namespace mwmp;

PacketObjectRestock::PacketObjectRestock(mwmp::NetworkManager *network) : ObjectPacket(network)
{
    packetID = ID_OBJECT_RESTOCK;
    hasCellData = true;
}
