#ifndef OPENMW_PACKETPLAYERSPELLSACTIVE_HPP
#define OPENMW_PACKETPLAYERSPELLSACTIVE_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerSpellsActive : public PlayerPacket
    {
    public:
        PacketPlayerSpellsActive(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);

    protected:
        static const int maxEffects = 20;
    };
}

#endif //OPENMW_PACKETPLAYERSPELLSACTIVE_HPP
