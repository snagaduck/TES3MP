#include "PlayerProcessor.hpp"
#include "Networking.hpp"
#include <components/openmw-mp/Net/RakNetManager.hpp>

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

bool PlayerProcessor::Process(RakNet::Packet &packet) noexcept
{
    for (auto &processor : processors)
    {
        if (processor.first == packet.data[0])
        {
            Player *player = Players::getPlayer(mwmp::RakNetManager::getInstance()->ToPlayerId(packet.guid));
            PlayerPacket *myPacket = Networking::get().getPlayerPacketController()->GetPacket(packet.data[0]);
            myPacket->setPlayer(player);

            if (!processor.second->avoidReading)
                myPacket->Read();

            processor.second->Do(*myPacket, *player);
            return true;
        }
    }
    return false;
}
