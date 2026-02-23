#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketPlayerSpellbook.hpp"

using namespace mwmp;

PacketPlayerSpellbook::PacketPlayerSpellbook(mwmp::NetworkManager *network) : PlayerPacket(network)
{
    packetID = ID_PLAYER_SPELLBOOK;
}

void PacketPlayerSpellbook::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    PlayerPacket::Packet(newBitstream, send);

    RW(player->spellbookChanges.action, send);

    uint32_t count;

    if (send)
        count = static_cast<uint32_t>(player->spellbookChanges.spells.size());

    RW(count, send);

    if (!send)
    {
        player->spellbookChanges.spells.clear();
        player->spellbookChanges.spells.resize(count);
    }

    for (auto &&spell : player->spellbookChanges.spells)
    {
        RW(spell.mId, send, true);
    }
}
