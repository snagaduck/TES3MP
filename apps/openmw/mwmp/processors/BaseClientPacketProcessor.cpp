#include "BaseClientPacketProcessor.hpp"
#include "../Main.hpp"

using namespace mwmp;

mwmp::PlayerId BaseClientPacketProcessor::guid;
mwmp::PlayerId BaseClientPacketProcessor::myGuid;
mwmp::PlayerId BaseClientPacketProcessor::serverPlayerId{mwmp::INVALID_PLAYER_ID};
bool BaseClientPacketProcessor::request;

LocalPlayer *BaseClientPacketProcessor::getLocalPlayer()
{
    return Main::get().getLocalPlayer();
}
