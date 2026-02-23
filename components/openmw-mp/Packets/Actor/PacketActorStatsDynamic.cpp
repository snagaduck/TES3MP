#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/TimedLog.hpp>
#include <components/esm/creaturestats.hpp>
#include "PacketActorStatsDynamic.hpp"

using namespace mwmp;

PacketActorStatsDynamic::PacketActorStatsDynamic(mwmp::NetworkManager *network) : ActorPacket(network)
{
    packetID = ID_ACTOR_STATS_DYNAMIC;
}

void PacketActorStatsDynamic::Actor(BaseActor &actor, bool send)
{
    RW(actor.creatureStats.mDynamic, send);

    actor.hasStatsDynamicData = true;
}
