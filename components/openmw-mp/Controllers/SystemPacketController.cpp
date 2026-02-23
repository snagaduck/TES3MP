#include "../Packets/System/PacketSystemHandshake.hpp"

#include "SystemPacketController.hpp"

template <typename T>
inline void AddPacket(mwmp::SystemPacketController::packets_t *packets, mwmp::NetworkManager *network)
{
    T *packet = new T(network);
    typedef mwmp::SystemPacketController::packets_t::value_type value_t;
    packets->insert(value_t(packet->GetPacketID(), value_t::second_type(packet)));
}

mwmp::SystemPacketController::SystemPacketController(mwmp::NetworkManager *network)
{
    AddPacket<PacketSystemHandshake>(&packets, network);
}


mwmp::SystemPacket *mwmp::SystemPacketController::GetPacket(unsigned char id)
{
    return packets[(unsigned char)id].get();
}

void mwmp::SystemPacketController::SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    for(const auto &packet : packets)
        packet.second->SetStreams(inStream, outStream);
}

bool mwmp::SystemPacketController::ContainsPacket(unsigned char id)
{
    for(const auto &packet : packets)
    {
        if (packet.first == id)
            return true;
    }
    return false;
}
