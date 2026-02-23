#include <components/openmw-mp/NetworkMessages.hpp>
#include "ActorPacket.hpp"

using namespace mwmp;

ActorPacket::ActorPacket(mwmp::NetworkManager *network) : BasePacket(network)
{
    packetID = 0;
    reliable = true;
    orderChannel = CHANNEL_ACTOR;
}

ActorPacket::~ActorPacket()
{

}

void ActorPacket::setActorList(BaseActorList *newActorList)
{
    actorList = newActorList;
    guid = actorList->guid;
}

void ActorPacket::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    if (!PacketHeader(newBitstream, send))
        return;

    BaseActor actor;

    for (unsigned int i = 0; i < actorList->count; i++)
    {
        if (send)
            actor = actorList->baseActors.at(i);

        RW(actor.refNum, send);
        RW(actor.mpNum, send);

        Actor(actor, send);

        if (!send)
            actorList->baseActors.push_back(actor);
    }
}

bool ActorPacket::PacketHeader(mwmp::NetBuffer *newBitstream, bool send)
{
    BasePacket::Packet(newBitstream, send);

    RW(actorList->cell.mData, send, true);
    RW(actorList->cell.mName, send, true);

    if (send)
        actorList->count = (unsigned int)(actorList->baseActors.size());
    else
        actorList->baseActors.clear();

    RW(actorList->count, send);

    if (actorList->count > maxActors)
    {
        actorList->isValid = false;
        return false;
    }

    return true;
}


void ActorPacket::Actor(BaseActor &actor, bool send)
{

}
