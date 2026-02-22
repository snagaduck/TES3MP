#include "../Networking.hpp"
#include "PlayerProcessor.hpp"
#include "../Main.hpp"

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

PlayerProcessor::~PlayerProcessor()
{

}

bool PlayerProcessor::Process(RakNet::Packet &packet)
{
    std::memcpy(&guid, packet.data + 1, sizeof(mwmp::PlayerId));
    const size_t hdrLen = 1 + sizeof(mwmp::PlayerId);
    mwmp::NetBuffer bsIn(packet.data + hdrLen, packet.length > hdrLen ? packet.length - hdrLen : 0);

    PlayerPacket *myPacket = Main::get().getNetworking()->getPlayerPacket(packet.data[0]);
    myPacket->SetReadStream(&bsIn);

    /*if (myPacket == 0)
    {
        // error: packet not found
    }*/

    for (auto &processor : processors)
    {
        if (processor.first == packet.data[0])
        {
            myGuid = Main::get().getLocalPlayer()->guid;
            request = packet.length == myPacket->headerSize();

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
