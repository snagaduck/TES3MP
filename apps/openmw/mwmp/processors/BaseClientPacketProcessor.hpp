#ifndef OPENMW_BASECLIENTPACKETPROCESSOR_HPP
#define OPENMW_BASECLIENTPACKETPROCESSOR_HPP

#include <components/openmw-mp/Base/BasePacketProcessor.hpp>
#include <components/openmw-mp/Net/PlayerId.hpp>
#include "../LocalPlayer.hpp"
#include "../DedicatedPlayer.hpp"

namespace mwmp
{
    class BaseClientPacketProcessor
    {
    public:
        static void SetServerPlayerId(mwmp::PlayerId pid)
        {
            serverPlayerId = pid;
        }

    protected:
        inline bool isRequest()
        {
            return request;
        }

        inline bool isLocal()
        {
            return guid == myGuid;
        }

        LocalPlayer *getLocalPlayer();

    protected:
        static mwmp::PlayerId guid, myGuid;
        static mwmp::PlayerId serverPlayerId;

        static bool request;
    };
}

#endif //OPENMW_BASECLIENTPACKETPROCESSOR_HPP
