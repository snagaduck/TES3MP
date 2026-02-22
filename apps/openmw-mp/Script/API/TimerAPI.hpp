#ifndef OPENMW_TIMERAPI_HPP
#define OPENMW_TIMERAPI_HPP

#include <string>
#include <unordered_map>

#include <Script/ScriptFunction.hpp>

namespace mwmp
{

    class TimerAPI;

    class Timer: public ScriptFunction
    {
        friend class TimerAPI;

    public:
#if defined(ENABLE_LUA)
        Timer(lua_State *lua, const std::string& callback, long msec, std::vector<sol::object> args);
#endif
        void Tick();

        bool IsEnded();
        void Stop();
        void Start();
        void Restart(int msec);
    private:
        double startTime, targetMsec;
        std::vector<sol::object> args;
        bool isEnded;
    };

    class TimerAPI
    {
    public:
#if defined(ENABLE_LUA)
        static int CreateTimerLua(lua_State *lua, const std::string& callback, long msec, std::vector<sol::object> args);
#endif
        static void FreeTimer(int timerid);
        static void ResetTimer(int timerid, long msec);
        static void StartTimer(int timerid);
        static void StopTimer(int timerid);
        static bool IsTimerElapsed(int timerid);

        static void Terminate();

        static void Tick();
    private:
        static std::unordered_map<int, Timer* > timers;
        static int pointer;
    };
}

#endif //OPENMW_TIMERAPI_HPP
