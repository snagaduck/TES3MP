#ifndef OPENMW_OBJECTPROCESSOR_HPP
#define OPENMW_OBJECTPROCESSOR_HPP


#include <components/openmw-mp/Base/BasePacketProcessor.hpp>
#include <components/openmw-mp/Packets/BasePacket.hpp>
#include <components/openmw-mp/Packets/Object/ObjectPacket.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/Net/ReceivedPacket.hpp>
#include "Script/Script.hpp"
#include "Player.hpp"

namespace mwmp
{
    class ObjectProcessor : public BasePacketProcessor<ObjectProcessor>
    {
    public:

        virtual void Do(ObjectPacket &packet, Player &player, BaseObjectList &objectList);

        static bool Process(mwmp::ReceivedPacket &packet, BaseObjectList &objectList) noexcept;
    };
}

#endif //OPENMW_OBJECTPROCESSOR_HPP
