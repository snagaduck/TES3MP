#ifndef OPENMW_PACKETPLAYERJAIL_HPP
#define OPENMW_PACKETPLAYERJAIL_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerJail : public PlayerPacket
    {
    public:
        PacketPlayerJail(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERJAIL_HPP
