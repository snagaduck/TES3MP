#include "../Main.hpp"
#include "../Networking.hpp"

#include "ObjectProcessor.hpp"

#include <components/openmw-mp/Net/ReceivedPacket.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

ObjectProcessor::~ObjectProcessor()
{

}

bool ObjectProcessor::Process(mwmp::ReceivedPacket &rp, ObjectList &objectList)
{
    guid = rp.sender;
    objectList.guid = guid;

    ObjectPacket *myPacket = Main::get().getNetworking()->getObjectPacket(rp.packetId);

    myPacket->setObjectList(&objectList);
    myPacket->SetReadStream(&rp.data);

    for (auto &processor: processors)
    {
        if (processor.first == rp.packetId)
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = rp.data.GetSize() == 0;

            objectList.isValid = true;

            if (!request && !processor.second->avoidReading)
                myPacket->Read();

            if (objectList.isValid)
                processor.second->Do(*myPacket, objectList);
            else
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Received %s that failed integrity check and was ignored!", processor.second->strPacketID.c_str());

            return true;
        }
    }
    return false;
}
