#include <components/openmw-mp/NetworkMessages.hpp>
#include "SystemPacket.hpp"

using namespace mwmp;

SystemPacket::SystemPacket(mwmp::NetworkManager *network) : BasePacket(network)
{
    packetID = 0;
    reliable = true;
    orderChannel = CHANNEL_SYSTEM;
}

SystemPacket::~SystemPacket()
{

}

void SystemPacket::setSystem(BaseSystem *newSystem)
{
    system = newSystem;
    guid = system->guid;
}

BaseSystem *SystemPacket::getSystem()
{
    return system;
}
