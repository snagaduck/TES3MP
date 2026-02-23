#include <components/openmw-mp/NetworkMessages.hpp>
#include "WorldstatePacket.hpp"

using namespace mwmp;

WorldstatePacket::WorldstatePacket(mwmp::NetworkManager *network) : BasePacket(network)
{
    packetID = 0;
    reliable = true;
    orderChannel = CHANNEL_WORLDSTATE;
}

WorldstatePacket::~WorldstatePacket()
{

}

void WorldstatePacket::setWorldstate(BaseWorldstate *newWorldstate)
{
    worldstate = newWorldstate;
    guid = worldstate->guid;
}

BaseWorldstate *WorldstatePacket::getWorldstate()
{
    return worldstate;
}
