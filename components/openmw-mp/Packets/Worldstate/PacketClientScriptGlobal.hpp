#ifndef OPENMW_PACKETCLIENTSCRIPTGLOBAL_HPP
#define OPENMW_PACKETCLIENTSCRIPTGLOBAL_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>

namespace mwmp
{
    class PacketClientScriptGlobal: public WorldstatePacket
    {
    public:
        PacketClientScriptGlobal(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETCLIENTSCRIPTGLOBAL_HPP
