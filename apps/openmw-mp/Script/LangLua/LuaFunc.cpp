#include <iostream>
#include "LangLua.hpp"
#include <Script/API/TimerAPI.hpp>
#include <Script/API/PublicFnAPI.hpp>

// Read Lua stack slots [args_begin, args_begin+args_n) as sol::objects.
inline std::vector<sol::object> StackToVec(lua_State *lua, int args_begin, int args_n)
{
    std::vector<sol::object> args;
    args.reserve(args_n);
    for (int i = args_begin; i < args_n + args_begin; i++)
        args.emplace_back(sol::stack::get<sol::object>(lua, i));
    return args;
}

int LangLua::MakePublic(lua_State *lua) noexcept
{
    const char *callback = sol::stack::get<const char*>(lua, 1);
    const char *name     = sol::stack::get<const char*>(lua, 2);
    // args 3 (ret_type) and 4 (def) accepted for backward compat but ignored

    Public::MakePublic(callback, lua, name);
    return 0;
}

int LangLua::CallPublic(lua_State *lua)
{
    const char *name = sol::stack::get<const char*>(lua, 1);
    int args_n = lua_gettop(lua) - 1;

    std::vector<sol::object> args = StackToVec(lua, 2, args_n);

    sol::object result = Public::Call(name, args);
    if (!result.valid() || result.get_type() == sol::type::nil)
        return 0;

    sol::stack::push(lua, result);
    return 1;
}

int LangLua::CreateTimer(lua_State *lua) noexcept
{
    const char *callback = sol::stack::get<const char*>(lua, 1);
    int msec = sol::stack::get<int>(lua, 2);

    int id = mwmp::TimerAPI::CreateTimerLua(lua, callback, msec, {});
    lua_pushinteger(lua, id);
    return 1;
}

int LangLua::CreateTimerEx(lua_State *lua)
{
    const char *callback = sol::stack::get<const char*>(lua, 1);
    int msec = sol::stack::get<int>(lua, 2);
    // arg 3 is the type string (accepted for backward compat, count derived from lua_gettop)
    int args_n = lua_gettop(lua) - 3;

    std::vector<sol::object> args = StackToVec(lua, 4, args_n);

    int id = mwmp::TimerAPI::CreateTimerLua(lua, callback, msec, std::move(args));
    lua_pushinteger(lua, id);
    return 1;
}
