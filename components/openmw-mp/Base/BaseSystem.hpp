#ifndef OPENMW_BASESYSTEM_HPP
#define OPENMW_BASESYSTEM_HPP

#include <string>

#include <components/openmw-mp/Net/PlayerId.hpp>

namespace mwmp
{
    class BaseSystem
    {
    public:

        BaseSystem(mwmp::PlayerId guid) : guid(guid)
        {

        }

        BaseSystem()
        {

        }

        mwmp::PlayerId guid;
        std::string playerName;
        std::string serverPassword;

    };
}

#endif //OPENMW_BASESYSTEM_HPP
