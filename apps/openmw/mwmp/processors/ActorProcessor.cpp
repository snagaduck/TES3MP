#include "ActorProcessor.hpp"
#include "../Networking.hpp"
#include "../Main.hpp"

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

ActorProcessor::~ActorProcessor()
{

}

bool ActorProcessor::Process(RakNet::Packet &packet, ActorList &actorList)
{
    std::memcpy(&guid, packet.data + 1, sizeof(mwmp::PlayerId));
    const size_t hdrLen = 1 + sizeof(mwmp::PlayerId);
    mwmp::NetBuffer bsIn(packet.data + hdrLen, packet.length > hdrLen ? packet.length - hdrLen : 0);
    actorList.guid = guid;

    ActorPacket *myPacket = Main::get().getNetworking()->getActorPacket(packet.data[0]);

    myPacket->setActorList(&actorList);
    myPacket->SetReadStream(&bsIn);

    for (auto &processor : processors)
    {
        if (processor.first == packet.data[0])
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = packet.length == myPacket->headerSize();

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
