#ifndef SCRIPTFUNCTIONS_HPP
#define SCRIPTFUNCTIONS_HPP

#include <Script/Functions/Actors.hpp>
#include <Script/Functions/Books.hpp>
#include <Script/Functions/Cells.hpp>
#include <Script/Functions/CharClass.hpp>
#include <Script/Functions/Chat.hpp>
#include <Script/Functions/Dialogue.hpp>
#include <Script/Functions/Factions.hpp>
#include <Script/Functions/GUI.hpp>
#include <Script/Functions/Items.hpp>
#include <Script/Functions/Mechanics.hpp>
#include <Script/Functions/Miscellaneous.hpp>
#include <Script/Functions/Objects.hpp>
#include <Script/Functions/Positions.hpp>
#include <Script/Functions/Quests.hpp>
#include <Script/Functions/RecordsDynamic.hpp>
#include <Script/Functions/Shapeshift.hpp>
#include <Script/Functions/Server.hpp>
#include <Script/Functions/Settings.hpp>
#include <Script/Functions/Spells.hpp>
#include <Script/Functions/Stats.hpp>
#include <Script/Functions/Worldstate.hpp>
#include <apps/openmw-mp/Player.hpp>
#include <apps/openmw-mp/Utils.hpp>
#include "ScriptFunction.hpp"

#include <components/openmw-mp/TimedLog.hpp>

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define GET_PLAYER(pid, pl, retvalue) \
     pl = Players::getPlayer(pid); \
     if (player == 0) {\
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, "%s: Player with pid \'%d\' not found\n", __PRETTY_FUNCTION__, pid);\
        /*ScriptFunctions::StopServer(1);*/ \
        return retvalue;\
}


class ScriptFunctions
{
public:

    /**
    * \brief Start the timer with a certain ID.
    *
    * \param timerId The timer ID.
    * \return void
    */
    static void StartTimer(int timerId) noexcept;

    /**
    * \brief Stop the timer with a certain ID.
    *
    * \param timerId The timer ID.
    * \return void
    */
    static void StopTimer(int timerId) noexcept;

    /**
    * \brief Restart the timer with a certain ID for a certain interval.
    *
    * \param timerId The timer ID.
    * \param msec The interval in miliseconds.
    * \return void
    */
    static void RestartTimer(int timerId, int msec) noexcept;

    /**
    * \brief Free the timer with a certain ID.
    *
    * \param timerId The timer ID.
    * \return void
    */
    static void FreeTimer(int timerId) noexcept;

    /**
    * \brief Check whether a timer is elapsed.
    *
    * \param timerId The timer ID.
    * \return Whether the timer is elapsed.
    */
    static bool IsTimerElapsed(int timerId) noexcept;


};

#endif //SCRIPTFUNCTIONS_HPP
