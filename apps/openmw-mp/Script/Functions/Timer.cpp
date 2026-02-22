#include <apps/openmw-mp/Script/ScriptFunctions.hpp>
#include <Script/API/TimerAPI.hpp>

using namespace mwmp;

void ScriptFunctions::StartTimer(int timerId) noexcept
{
    TimerAPI::StartTimer(timerId);
}

void ScriptFunctions::StopTimer(int timerId) noexcept
{
    TimerAPI::StopTimer(timerId);
}

void ScriptFunctions::RestartTimer(int timerId, int msec) noexcept
{
    TimerAPI::ResetTimer(timerId, msec);
}

void ScriptFunctions::FreeTimer(int timerId) noexcept
{
    TimerAPI::FreeTimer(timerId);
}

bool ScriptFunctions::IsTimerElapsed(int timerId) noexcept
{
    return TimerAPI::IsTimerElapsed(timerId);
}
