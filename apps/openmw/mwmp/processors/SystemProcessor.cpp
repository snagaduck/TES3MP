#include "../Networking.hpp"
#include "SystemProcessor.hpp"
#include "../Main.hpp"

#include <components/openmw-mp/Net/ReceivedPacket.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

SystemProcessor::~SystemProcessor()
{

}

bool SystemProcessor::Process(mwmp::ReceivedPacket &rp)
{
    guid = rp.sender;

    SystemPacket *myPacket = Main::get().getNetworking()->getSystemPacket(rp.packetId);
    myPacket->SetReadStream(&rp.data);

    for (auto &processor : processors)
    {
        if (processor.first == rp.packetId)
        {
            myGuid = Main::get().getLocalSystem()->guid;
            request = rp.data.GetSize() == 0;

            BaseSystem *system = 0;
            system = Main::get().getLocalSystem();

            if (!request && !processor.second->avoidReading && system != 0)
            {
                myPacket->setSystem(system);
                myPacket->Read();
            }

            processor.second->Do(*myPacket, system);
            return true;
        }
    }
    return false;
}
