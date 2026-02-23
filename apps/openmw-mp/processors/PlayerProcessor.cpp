#include "PlayerProcessor.hpp"
#include "Networking.hpp"

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

bool PlayerProcessor::Process(mwmp::ReceivedPacket &packet) noexcept
{
    for (auto &processor : processors)
    {
        if (processor.first == packet.packetId)
        {
            Player *player = Players::getPlayer(packet.sender);
            PlayerPacket *myPacket = Networking::get().getPlayerPacketController()->GetPacket(packet.packetId);
            myPacket->setPlayer(player);

            if (!processor.second->avoidReading)
                myPacket->Read();

            processor.second->Do(*myPacket, *player);
            return true;
        }
    }
    return false;
}
