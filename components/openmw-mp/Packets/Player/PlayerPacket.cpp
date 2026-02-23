#include <components/openmw-mp/NetworkMessages.hpp>
#include "PlayerPacket.hpp"

using namespace mwmp;

PlayerPacket::PlayerPacket(mwmp::NetworkManager *network) : BasePacket(network)
{
    packetID = 0;
    reliable = true;
    orderChannel = CHANNEL_PLAYER;
}

PlayerPacket::~PlayerPacket()
{

}

void PlayerPacket::setPlayer(BasePlayer *newPlayer)
{
    player = newPlayer;
    guid = player->guid;
}

BasePlayer *PlayerPacket::getPlayer()
{
    return player;
}
