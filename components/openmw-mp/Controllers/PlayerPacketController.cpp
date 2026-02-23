#include "../Packets/Player/PacketDisconnect.hpp"
#include "../Packets/Player/PacketChatMessage.hpp"
#include "../Packets/Player/PacketPlayerCharGen.hpp"
#include "../Packets/Player/PacketGUIBoxes.hpp"
#include "../Packets/Player/PacketLoaded.hpp"
#include "../Packets/Player/PacketGameSettings.hpp"
#include "../Packets/Player/PacketPlayerSpellsActive.hpp"
#include "../Packets/Player/PacketPlayerAlly.hpp"
#include "../Packets/Player/PacketPlayerAnimFlags.hpp"
#include "../Packets/Player/PacketPlayerAnimPlay.hpp"
#include "../Packets/Player/PacketPlayerAttack.hpp"
#include "../Packets/Player/PacketPlayerAttribute.hpp"
#include "../Packets/Player/PacketPlayerBaseInfo.hpp"
#include "../Packets/Player/PacketPlayerBehavior.hpp"
#include "../Packets/Player/PacketPlayerBook.hpp"
#include "../Packets/Player/PacketPlayerBounty.hpp"
#include "../Packets/Player/PacketPlayerCast.hpp"
#include "../Packets/Player/PacketPlayerCellChange.hpp"
#include "../Packets/Player/PacketPlayerCellState.hpp"
#include "../Packets/Player/PacketPlayerClass.hpp"
#include "../Packets/Player/PacketPlayerCooldowns.hpp"
#include "../Packets/Player/PacketPlayerDeath.hpp"
#include "../Packets/Player/PacketPlayerEquipment.hpp"
#include "../Packets/Player/PacketPlayerFaction.hpp"
#include "../Packets/Player/PacketPlayerInput.hpp"
#include "../Packets/Player/PacketPlayerInventory.hpp"
#include "../Packets/Player/PacketPlayerItemUse.hpp"
#include "../Packets/Player/PacketPlayerJail.hpp"
#include "../Packets/Player/PacketPlayerJournal.hpp"
#include "../Packets/Player/PacketPlayerLevel.hpp"
#include "../Packets/Player/PacketPlayerMiscellaneous.hpp"
#include "../Packets/Player/PacketPlayerMomentum.hpp"
#include "../Packets/Player/PacketPlayerPosition.hpp"
#include "../Packets/Player/PacketPlayerQuickKeys.hpp"
#include "../Packets/Player/PacketPlayerReputation.hpp"
#include "../Packets/Player/PacketPlayerRest.hpp"
#include "../Packets/Player/PacketPlayerResurrect.hpp"
#include "../Packets/Player/PacketPlayerShapeshift.hpp"
#include "../Packets/Player/PacketPlayerSkill.hpp"
#include "../Packets/Player/PacketPlayerSpeech.hpp"
#include "../Packets/Player/PacketPlayerSpellbook.hpp"
#include "../Packets/Player/PacketPlayerStatsDynamic.hpp"
#include "../Packets/Player/PacketPlayerTopic.hpp"

#include "PlayerPacketController.hpp"

template <typename T>
inline void AddPacket(mwmp::PlayerPacketController::packets_t *packets, mwmp::NetworkManager *network)
{
    T *packet = new T(network);
    typedef mwmp::PlayerPacketController::packets_t::value_type value_t;
    packets->insert(value_t(packet->GetPacketID(), value_t::second_type(packet)));
}

mwmp::PlayerPacketController::PlayerPacketController(mwmp::NetworkManager *network)
{
    AddPacket<PacketDisconnect>(&packets, network);
    AddPacket<PacketChatMessage>(&packets, network);
    AddPacket<PacketGUIBoxes>(&packets, network);
    AddPacket<PacketLoaded>(&packets, network);
    AddPacket<PacketGameSettings>(&packets, network);
    AddPacket<PacketPlayerSpellsActive>(&packets, network);

    AddPacket<PacketPlayerAlly>(&packets, network);
    AddPacket<PacketPlayerAnimFlags>(&packets, network);
    AddPacket<PacketPlayerAnimPlay>(&packets, network);
    AddPacket<PacketPlayerAttack>(&packets, network);
    AddPacket<PacketPlayerAttribute>(&packets, network);
    AddPacket<PacketPlayerBaseInfo>(&packets, network);
    AddPacket<PacketPlayerBehavior>(&packets, network);
    AddPacket<PacketPlayerBook>(&packets, network);
    AddPacket<PacketPlayerBounty>(&packets, network);
    AddPacket<PacketPlayerCast>(&packets, network);
    AddPacket<PacketPlayerCellChange>(&packets, network);
    AddPacket<PacketPlayerCellState>(&packets, network);
    AddPacket<PacketPlayerCharGen>(&packets, network);
    AddPacket<PacketPlayerClass>(&packets, network);
    AddPacket<PacketPlayerCooldowns>(&packets, network);
    AddPacket<PacketPlayerDeath>(&packets, network);
    AddPacket<PacketPlayerEquipment>(&packets, network);
    AddPacket<PacketPlayerFaction>(&packets, network);
    AddPacket<PacketPlayerInput>(&packets, network);
    AddPacket<PacketPlayerInventory>(&packets, network);
    AddPacket<PacketPlayerItemUse>(&packets, network);
    AddPacket<PacketPlayerJail>(&packets, network);
    AddPacket<PacketPlayerJournal>(&packets, network);
    AddPacket<PacketPlayerLevel>(&packets, network);
    AddPacket<PacketPlayerMiscellaneous>(&packets, network);
    AddPacket<PacketPlayerMomentum>(&packets, network);
    AddPacket<PacketPlayerPosition>(&packets, network);
    AddPacket<PacketPlayerQuickKeys>(&packets, network);
    AddPacket<PacketPlayerReputation>(&packets, network);
    AddPacket<PacketPlayerRest>(&packets, network);
    AddPacket<PacketPlayerResurrect>(&packets, network);
    AddPacket<PacketPlayerShapeshift>(&packets, network);
    AddPacket<PacketPlayerSkill>(&packets, network);
    AddPacket<PacketPlayerSpeech>(&packets, network);
    AddPacket<PacketPlayerSpellbook>(&packets, network);
    AddPacket<PacketPlayerStatsDynamic>(&packets, network);
    AddPacket<PacketPlayerTopic>(&packets, network);
}


mwmp::PlayerPacket *mwmp::PlayerPacketController::GetPacket(unsigned char id)
{
    return packets[(unsigned char)id].get();
}

void mwmp::PlayerPacketController::SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    for(const auto &packet : packets)
        packet.second->SetStreams(inStream, outStream);
}

bool mwmp::PlayerPacketController::ContainsPacket(unsigned char id)
{
    for(const auto &packet : packets)
    {
        if (packet.first == id)
            return true;
    }
    return false;
}
