#include "ActorProcessor.hpp"
#include "Networking.hpp"

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

void ActorProcessor::Do(ActorPacket &packet, Player &player, BaseActorList &actorList)
{
    packet.Send(true);
}

bool ActorProcessor::Process(mwmp::ReceivedPacket &packet, BaseActorList &actorList) noexcept
{
    // Clear our BaseActorList before loading new data in it
    actorList.cell.blank();
    actorList.baseActors.clear();
    actorList.guid = packet.sender;

    for (auto &processor : processors)
    {
        if (processor.first == packet.packetId)
        {
            Player *player = Players::getPlayer(actorList.guid);
            ActorPacket *myPacket = Networking::get().getActorPacketController()->GetPacket(packet.packetId);

            myPacket->setActorList(&actorList);
            actorList.isValid = true;

            if (!processor.second->avoidReading)
                myPacket->Read();

            if (actorList.isValid)
                processor.second->Do(*myPacket, *player, actorList);
            else
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Received %s that failed integrity check and was ignored!", processor.second->strPacketID.c_str());

            return true;
        }
    }
    return false;
}
