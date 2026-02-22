#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketPlayerSpeech.hpp"

mwmp::PacketPlayerSpeech::PacketPlayerSpeech(RakNet::RakPeerInterface *peer) : PlayerPacket(peer)
{
    packetID = ID_PLAYER_SPEECH;
}

void mwmp::PacketPlayerSpeech::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->sound, send);
}
