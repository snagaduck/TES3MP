#ifndef OPENMW_PACKETPLAYERINPUT_HPP
#define OPENMW_PACKETPLAYERINPUT_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerInput : public PlayerPacket
    {
    public:
        PacketPlayerInput(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERINPUT_HPP
