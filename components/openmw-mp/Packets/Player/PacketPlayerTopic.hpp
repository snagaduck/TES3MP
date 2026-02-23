#ifndef OPENMW_PACKETPLAYERTOPIC_HPP
#define OPENMW_PACKETPLAYERTOPIC_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerTopic : public PlayerPacket
    {
    public:
        PacketPlayerTopic(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERTOPIC_HPP
