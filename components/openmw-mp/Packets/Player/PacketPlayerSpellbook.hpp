#ifndef OPENMW_PACKETPLAYERSPELLBOOK_HPP
#define OPENMW_PACKETPLAYERSPELLBOOK_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerSpellbook : public PlayerPacket
    {
    public:
        PacketPlayerSpellbook(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERSPELLBOOK_HPP
