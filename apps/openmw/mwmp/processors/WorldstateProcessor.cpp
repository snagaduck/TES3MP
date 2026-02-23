#include "../Main.hpp"
#include "../Networking.hpp"

#include "WorldstateProcessor.hpp"

#include <components/openmw-mp/Net/ReceivedPacket.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

WorldstateProcessor::~WorldstateProcessor()
{

}

bool WorldstateProcessor::Process(mwmp::ReceivedPacket &rp, Worldstate &worldstate)
{
    guid = rp.sender;
    worldstate.guid = guid;

    WorldstatePacket *myPacket = Main::get().getNetworking()->getWorldstatePacket(rp.packetId);

    myPacket->setWorldstate(&worldstate);
    myPacket->SetReadStream(&rp.data);

    for (auto &processor : processors)
    {
        if (processor.first == rp.packetId)
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = rp.data.GetSize() == 0;

            worldstate.isValid = true;

            if (!request && !processor.second->avoidReading)
                myPacket->Read();

            if (worldstate.isValid)
                processor.second->Do(*myPacket, worldstate);
            else
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Received %s that failed integrity check and was ignored!", processor.second->strPacketID.c_str());

            return true;
        }
    }
    return false;
}
