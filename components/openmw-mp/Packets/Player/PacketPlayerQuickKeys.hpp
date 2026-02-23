#ifndef OPENMW_PACKETPLAYERQUICKKEYS_HPP
#define OPENMW_PACKETPLAYERQUICKKEYS_HPP

#include <components/openmw-mp/Packets/Player/PlayerPacket.hpp>

namespace mwmp
{
    class PacketPlayerQuickKeys : public PlayerPacket
    {
    public:
        PacketPlayerQuickKeys(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETPLAYERQUICKKEYS_HPP
