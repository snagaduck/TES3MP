#include "ScriptFunction.hpp"

#if defined(ENABLE_LUA)
#include "LangLua/LangLua.hpp"

ScriptFunction::ScriptFunction(const std::string& funcName, lua_State* lua)
    : lua(lua), funcName(funcName)
{}

sol::object ScriptFunction::Call(const std::vector<sol::object>& args)
{
    lua_getglobal(lua, funcName.c_str());
    for (const auto& arg : args)
        sol::stack::push(lua, arg);
    int n = (int)args.size();
    if (lua_pcall(lua, n, 1, 0) != 0)
    {
        std::string err = lua_tostring(lua, -1);
        lua_pop(lua, 1);
        throw std::runtime_error("Lua error in " + funcName + ": " + err);
    }
    sol::object result = sol::stack::get<sol::object>(lua, -1);
    lua_pop(lua, 1);
    return result;
}
#endif

ScriptFunction::~ScriptFunction() {}
