#ifndef OPENMW_PLAYERID_HPP
#define OPENMW_PLAYERID_HPP

#include <cstdint>

namespace mwmp
{
    using PlayerId = uint64_t;
    constexpr PlayerId INVALID_PLAYER_ID = 0;
}

#endif // OPENMW_PLAYERID_HPP
