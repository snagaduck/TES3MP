#include "ActorProcessor.hpp"
#include "../Networking.hpp"
#include "../Main.hpp"

#include <components/openmw-mp/Net/ReceivedPacket.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

ActorProcessor::~ActorProcessor()
{

}

bool ActorProcessor::Process(mwmp::ReceivedPacket &rp, ActorList &actorList)
{
    guid = rp.sender;
    actorList.guid = guid;

    ActorPacket *myPacket = Main::get().getNetworking()->getActorPacket(rp.packetId);

    myPacket->setActorList(&actorList);
    myPacket->SetReadStream(&rp.data);

    for (auto &processor : processors)
    {
        if (processor.first == rp.packetId)
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = rp.data.GetSize() == 0;

            actorList.isValid = true;

            if (!request && !processor.second->avoidReading)
            {
                myPacket->Read();
            }

            if (actorList.isValid)
                processor.second->Do(*myPacket, actorList);
            else
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Received %s that failed integrity check and was ignored!", processor.second->strPacketID.c_str());

            return true;
        }
    }
    return false;
}
