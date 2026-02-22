#include "BaseClientPacketProcessor.hpp"
#include "../Main.hpp"

using namespace mwmp;

mwmp::PlayerId BaseClientPacketProcessor::guid;
mwmp::PlayerId BaseClientPacketProcessor::myGuid;
RakNet::SystemAddress BaseClientPacketProcessor::serverAddr;
bool BaseClientPacketProcessor::request;

LocalPlayer *BaseClientPacketProcessor::getLocalPlayer()
{
    return Main::get().getLocalPlayer();
}
