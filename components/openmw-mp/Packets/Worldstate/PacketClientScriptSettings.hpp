#ifndef OPENMW_PACKETCLIENTSCRIPTSETTINGS_HPP
#define OPENMW_PACKETCLIENTSCRIPTSETTINGS_HPP

#include <components/openmw-mp/Packets/Worldstate/WorldstatePacket.hpp>

namespace mwmp
{
    class PacketClientScriptSettings : public WorldstatePacket
    {
    public:
        PacketClientScriptSettings(mwmp::NetworkManager *network);

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
    };
}

#endif //OPENMW_PACKETCLIENTSCRIPTSETTINGS_HPP
