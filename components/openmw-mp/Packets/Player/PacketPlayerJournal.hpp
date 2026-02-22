#ifndef OPENMW_PACKETPLAYERJOURNAL_HPP
#define OPENMW_PACKETPLAYERJOURNAL_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerJournal : public PlayerPacket
    {
    public:
        PacketPlayerJournal(RakNet::RakPeerInterface *peer);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERJOURNAL_HPP
