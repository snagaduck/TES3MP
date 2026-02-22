#ifndef OPENMW_PACKETPLAYERSPEECH_HPP
#define OPENMW_PACKETPLAYERSPEECH_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerSpeech : public PlayerPacket
    {
    public:
        PacketPlayerSpeech(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERSPEECH_HPP
