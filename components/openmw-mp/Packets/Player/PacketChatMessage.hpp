#ifndef OPENMW_PACKETCHATMESSAGE_HPP
#define OPENMW_PACKETCHATMESSAGE_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketChatMessage : public PlayerPacket
    {
    public:
        PacketChatMessage(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETCHATMESSAGE_HPP
