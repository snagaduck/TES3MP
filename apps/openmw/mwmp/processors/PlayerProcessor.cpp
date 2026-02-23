#include "../Networking.hpp"
#include "PlayerProcessor.hpp"
#include "../Main.hpp"

#include <components/openmw-mp/Net/ReceivedPacket.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

PlayerProcessor::~PlayerProcessor()
{

}

bool PlayerProcessor::Process(mwmp::ReceivedPacket &rp)
{
    guid = rp.sender;

    PlayerPacket *myPacket = Main::get().getNetworking()->getPlayerPacket(rp.packetId);
    myPacket->SetReadStream(&rp.data);

    for (auto &processor : processors)
    {
        if (processor.first == rp.packetId)
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = rp.data.GetSize() == 0;

            BasePlayer *player = 0;
            if (guid != myGuid)
                player = PlayerList::getPlayer(guid);
            else
                player = Main::get().getLocalPlayer();

            if (!request && !processor.second->avoidReading && player != 0)
            {
                myPacket->setPlayer(player);
                myPacket->Read();
            }

            processor.second->Do(*myPacket, player);
            return true;
        }
    }
    return false;
}
