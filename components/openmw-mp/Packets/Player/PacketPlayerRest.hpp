#ifndef OPENMW_PACKETPLAYERREST_HPP
#define OPENMW_PACKETPLAYERREST_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerRest : public PlayerPacket
    {
    public:
        PacketPlayerRest(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERREST_HPP
