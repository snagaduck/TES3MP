#ifndef OPENMW_PACKETPLAYERBOUNTY_HPP
#define OPENMW_PACKETPLAYERBOUNTY_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerBounty : public PlayerPacket
    {
    public:
        PacketPlayerBounty(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERBOUNTY_HPP
