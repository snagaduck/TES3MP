#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketObjectDelete.hpp"

using namespace mwmp;

PacketObjectDelete::PacketObjectDelete(mwmp::NetworkManager *network) : ObjectPacket(network)
{
    packetID = ID_OBJECT_DELETE;
    hasCellData = true;
}
