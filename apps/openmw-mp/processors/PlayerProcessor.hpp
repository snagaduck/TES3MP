#ifndef OPENMW_BASEPLAYERPROCESSOR_HPP
#define OPENMW_BASEPLAYERPROCESSOR_HPP

#include <components/openmw-mp/Base/BasePacketProcessor.hpp>
#include <components/openmw-mp/Packets/BasePacket.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>
#include "Player.hpp"

namespace mwmp
{
    class PlayerProcessor : public BasePacketProcessor<PlayerProcessor>
    {
    public:

        virtual void Do(PlayerPacket &packet, Player &player) = 0;

        static bool Process(mwmp::ReceivedPacket &packet) noexcept;
    };
}

#endif //OPENMW_BASEPLAYERPROCESSOR_HPP
