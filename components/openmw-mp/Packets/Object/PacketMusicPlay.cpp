#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketMusicPlay.hpp"

using namespace mwmp;

PacketMusicPlay::PacketMusicPlay(mwmp::NetworkManager *network) : ObjectPacket(network)
{
    packetID = ID_MUSIC_PLAY;
}

void PacketMusicPlay::Object(BaseObject &baseObject, bool send)
{
    RW(baseObject.musicFilename, send);
}
