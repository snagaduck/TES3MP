#include "../Packets/Object/PacketObjectActivate.hpp"
#include "../Packets/Object/PacketObjectAnimPlay.hpp"
#include "../Packets/Object/PacketObjectAttach.hpp"
#include "../Packets/Object/PacketObjectDelete.hpp"
#include "../Packets/Object/PacketObjectDialogueChoice.hpp"
#include "../Packets/Object/PacketObjectHit.hpp"
#include "../Packets/Object/PacketObjectLock.hpp"
#include "../Packets/Object/PacketObjectMiscellaneous.hpp"
#include "../Packets/Object/PacketObjectMove.hpp"
#include "../Packets/Object/PacketObjectPlace.hpp"
#include "../Packets/Object/PacketObjectRestock.hpp"
#include "../Packets/Object/PacketObjectRotate.hpp"
#include "../Packets/Object/PacketObjectScale.hpp"
#include "../Packets/Object/PacketObjectSound.hpp"
#include "../Packets/Object/PacketObjectSpawn.hpp"
#include "../Packets/Object/PacketObjectState.hpp"
#include "../Packets/Object/PacketObjectTrap.hpp"

#include "../Packets/Object/PacketContainer.hpp"
#include "../Packets/Object/PacketDoorDestination.hpp"
#include "../Packets/Object/PacketDoorState.hpp"
#include "../Packets/Object/PacketMusicPlay.hpp"
#include "../Packets/Object/PacketVideoPlay.hpp"

#include "../Packets/Object/PacketConsoleCommand.hpp"
#include "../Packets/Object/PacketClientScriptLocal.hpp"
#include "../Packets/Object/PacketScriptMemberShort.hpp"

#include "ObjectPacketController.hpp"

template <typename T>
inline void AddPacket(mwmp::ObjectPacketController::packets_t *packets, mwmp::NetworkManager *network)
{
    T *packet = new T(network);
    typedef mwmp::ObjectPacketController::packets_t::value_type value_t;
    packets->insert(value_t(packet->GetPacketID(), value_t::second_type(packet)));
}

mwmp::ObjectPacketController::ObjectPacketController(mwmp::NetworkManager *network)
{
    AddPacket<PacketObjectActivate>(&packets, network);
    AddPacket<PacketObjectAnimPlay>(&packets, network);
    AddPacket<PacketObjectAttach>(&packets, network);
    AddPacket<PacketObjectDelete>(&packets, network);
    AddPacket<PacketObjectDialogueChoice>(&packets, network);
    AddPacket<PacketObjectHit>(&packets, network);
    AddPacket<PacketObjectLock>(&packets, network);
    AddPacket<PacketObjectMiscellaneous>(&packets, network);
    AddPacket<PacketObjectMove>(&packets, network);
    AddPacket<PacketObjectPlace>(&packets, network);
    AddPacket<PacketObjectRestock>(&packets, network);
    AddPacket<PacketObjectRotate>(&packets, network);
    AddPacket<PacketObjectScale>(&packets, network);
    AddPacket<PacketObjectSound>(&packets, network);
    AddPacket<PacketObjectSpawn>(&packets, network);
    AddPacket<PacketObjectState>(&packets, network);
    AddPacket<PacketObjectTrap>(&packets, network);
    
    AddPacket<PacketContainer>(&packets, network);
    AddPacket<PacketDoorDestination>(&packets, network);
    AddPacket<PacketDoorState>(&packets, network);
    AddPacket<PacketMusicPlay>(&packets, network);
    AddPacket<PacketVideoPlay>(&packets, network);

    AddPacket<PacketConsoleCommand>(&packets, network);
    AddPacket<PacketClientScriptLocal>(&packets, network);
    AddPacket<PacketScriptMemberShort>(&packets, network);
}


mwmp::ObjectPacket *mwmp::ObjectPacketController::GetPacket(unsigned char id)
{
    return packets[(unsigned char)id].get();
}

void mwmp::ObjectPacketController::SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    for(const auto &packet : packets)
        packet.second->SetStreams(inStream, outStream);
}

bool mwmp::ObjectPacketController::ContainsPacket(unsigned char id)
{
    for(const auto &packet : packets)
    {
        if (packet.first == id)
            return true;
    }
    return false;
}
