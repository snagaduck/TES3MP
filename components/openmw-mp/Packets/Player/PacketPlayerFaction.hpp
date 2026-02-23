#ifndef OPENMW_PACKETPLAYERFACTION_HPP
#define OPENMW_PACKETPLAYERFACTION_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerFaction : public PlayerPacket
    {
    public:
        PacketPlayerFaction(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERFACTION_HPP
