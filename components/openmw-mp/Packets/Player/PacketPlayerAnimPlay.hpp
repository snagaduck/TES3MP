#ifndef OPENMW_PACKETPLAYERANIMPLAY_HPP
#define OPENMW_PACKETPLAYERANIMPLAY_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerAnimPlay : public PlayerPacket
    {
    public:
        PacketPlayerAnimPlay(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERANIMPLAY_HPP
