#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketObjectAttach.hpp"

using namespace mwmp;

PacketObjectAttach::PacketObjectAttach(mwmp::NetworkManager *network) : ObjectPacket(network)
{
    packetID = ID_OBJECT_ATTACH;
    hasCellData = true;
}

void PacketObjectAttach::Object(BaseObject &baseObject, bool send)
{
    ObjectPacket::Object(baseObject, send);
    // Placeholder
}
