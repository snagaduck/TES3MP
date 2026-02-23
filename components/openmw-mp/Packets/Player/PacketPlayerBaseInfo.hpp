#ifndef OPENMW_PACKETPLAYERBASEINFO_HPP
#define OPENMW_PACKETPLAYERBASEINFO_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerBaseInfo : public PlayerPacket
    {
    public:
        PacketPlayerBaseInfo(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERBASEINFO_HPP
