#ifndef OPENMW_PACKETPLAYERSPEECH_HPP
#define OPENMW_PACKETPLAYERSPEECH_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerSpeech : public PlayerPacket
    {
    public:
        PacketPlayerSpeech(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERSPEECH_HPP
