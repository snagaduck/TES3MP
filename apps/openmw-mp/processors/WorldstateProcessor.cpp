#include "WorldstateProcessor.hpp"
#include "Networking.hpp"

using namespace mwmp;

template<class T>
typename BasePacketProcessor<T>::processors_t BasePacketProcessor<T>::processors;

void WorldstateProcessor::Do(WorldstatePacket &packet, Player &player, BaseWorldstate &worldstate)
{
    packet.Send(true);
}

bool WorldstateProcessor::Process(mwmp::ReceivedPacket &packet, BaseWorldstate &worldstate) noexcept
{
    worldstate.guid = packet.sender;

    for (auto &processor : processors)
    {
        if (processor.first == packet.packetId)
        {
            Player *player = Players::getPlayer(worldstate.guid);
            WorldstatePacket *myPacket = Networking::get().getWorldstatePacketController()->GetPacket(packet.packetId);

            myPacket->setWorldstate(&worldstate);
            worldstate.isValid = true;

            if (!processor.second->avoidReading)
                myPacket->Read();

            if (worldstate.isValid)
                processor.second->Do(*myPacket, *player, worldstate);
            else
                LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "Received %s that failed integrity check and was ignored!", processor.second->strPacketID.c_str());

            return true;
        }
    }
    return false;
}
