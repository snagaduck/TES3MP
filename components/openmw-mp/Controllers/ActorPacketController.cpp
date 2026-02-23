#include "../Packets/Actor/PacketActorList.hpp"
#include "../Packets/Actor/PacketActorAuthority.hpp"
#include "../Packets/Actor/PacketActorTest.hpp"
#include "../Packets/Actor/PacketActorAI.hpp"
#include "../Packets/Actor/PacketActorAnimFlags.hpp"
#include "../Packets/Actor/PacketActorAnimPlay.hpp"
#include "../Packets/Actor/PacketActorAttack.hpp"
#include "../Packets/Actor/PacketActorCast.hpp"
#include "../Packets/Actor/PacketActorCellChange.hpp"
#include "../Packets/Actor/PacketActorDeath.hpp"
#include "../Packets/Actor/PacketActorEquipment.hpp"
#include "../Packets/Actor/PacketActorPosition.hpp"
#include "../Packets/Actor/PacketActorSpeech.hpp"
#include "../Packets/Actor/PacketActorSpellsActive.hpp"
#include "../Packets/Actor/PacketActorStatsDynamic.hpp"


#include "ActorPacketController.hpp"

template <typename T>
inline void AddPacket(mwmp::ActorPacketController::packets_t *packets, mwmp::NetworkManager *network)
{
    T *packet = new T(network);
    typedef mwmp::ActorPacketController::packets_t::value_type value_t;
    packets->insert(value_t(packet->GetPacketID(), value_t::second_type(packet)));
}

mwmp::ActorPacketController::ActorPacketController(mwmp::NetworkManager *network)
{
    AddPacket<PacketActorList>(&packets, network);
    AddPacket<PacketActorAuthority>(&packets, network);
    AddPacket<PacketActorTest>(&packets, network);
    AddPacket<PacketActorAI>(&packets, network);
    AddPacket<PacketActorAnimFlags>(&packets, network);
    AddPacket<PacketActorAnimPlay>(&packets, network);
    AddPacket<PacketActorAttack>(&packets, network);
    AddPacket<PacketActorCast>(&packets, network);
    AddPacket<PacketActorCellChange>(&packets, network);
    AddPacket<PacketActorDeath>(&packets, network);
    AddPacket<PacketActorEquipment>(&packets, network);
    AddPacket<PacketActorPosition>(&packets, network);
    AddPacket<PacketActorSpeech>(&packets, network);
    AddPacket<PacketActorSpellsActive>(&packets, network);
    AddPacket<PacketActorStatsDynamic>(&packets, network);
}


mwmp::ActorPacket *mwmp::ActorPacketController::GetPacket(unsigned char id)
{
    return packets[(unsigned char)id].get();
}

void mwmp::ActorPacketController::SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    for(const auto &packet : packets)
        packet.second->SetStreams(inStream, outStream);
}

bool mwmp::ActorPacketController::ContainsPacket(unsigned char id)
{
    for(const auto &packet : packets)
    {
        if (packet.first == id)
            return true;
    }
    return false;
}
