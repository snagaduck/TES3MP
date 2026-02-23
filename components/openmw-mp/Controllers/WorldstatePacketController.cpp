#include "../Packets/Worldstate/PacketCellReset.hpp"
#include "../Packets/Worldstate/PacketClientScriptGlobal.hpp"
#include "../Packets/Worldstate/PacketClientScriptSettings.hpp"
#include "../Packets/Worldstate/PacketRecordDynamic.hpp"
#include "../Packets/Worldstate/PacketWorldCollisionOverride.hpp"
#include "../Packets/Worldstate/PacketWorldDestinationOverride.hpp"
#include "../Packets/Worldstate/PacketWorldKillCount.hpp"
#include "../Packets/Worldstate/PacketWorldMap.hpp"
#include "../Packets/Worldstate/PacketWorldRegionAuthority.hpp"
#include "../Packets/Worldstate/PacketWorldTime.hpp"
#include "../Packets/Worldstate/PacketWorldWeather.hpp"

#include "WorldstatePacketController.hpp"

template <typename T>
inline void AddPacket(mwmp::WorldstatePacketController::packets_t *packets, mwmp::NetworkManager *network)
{
    T *packet = new T(network);
    typedef mwmp::WorldstatePacketController::packets_t::value_type value_t;
    packets->insert(value_t(packet->GetPacketID(), value_t::second_type(packet)));
}

mwmp::WorldstatePacketController::WorldstatePacketController(mwmp::NetworkManager *network)
{
    AddPacket<PacketCellReset>(&packets, network);
    AddPacket<PacketClientScriptGlobal>(&packets, network);
    AddPacket<PacketClientScriptSettings>(&packets, network);
    AddPacket<PacketRecordDynamic>(&packets, network);
    AddPacket<PacketWorldCollisionOverride>(&packets, network);
    AddPacket<PacketWorldDestinationOverride>(&packets, network);
    AddPacket<PacketWorldKillCount>(&packets, network);
    AddPacket<PacketWorldMap>(&packets, network);
    AddPacket<PacketWorldRegionAuthority>(&packets, network);
    AddPacket<PacketWorldTime>(&packets, network);
    AddPacket<PacketWorldWeather>(&packets, network);
}


mwmp::WorldstatePacket *mwmp::WorldstatePacketController::GetPacket(unsigned char id)
{
    return packets[(unsigned char)id].get();
}

void mwmp::WorldstatePacketController::SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    for(const auto &packet : packets)
        packet.second->SetStreams(inStream, outStream);
}

bool mwmp::WorldstatePacketController::ContainsPacket(unsigned char id)
{
    for(const auto &packet : packets)
    {
        if (packet.first == id)
            return true;
    }
    return false;
}
