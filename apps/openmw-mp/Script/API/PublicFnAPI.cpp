#include <Script/ScriptFunction.hpp>
#include "PublicFnAPI.hpp"

std::unordered_map<std::string, Public *> Public::publics;

Public::~Public()
{

}

#if defined(ENABLE_LUA)
Public::Public(const std::string& callback, lua_State *lua, const std::string &name)
    : ScriptFunction(callback, lua)
{
    publics.emplace(name, this);
}
#endif

sol::object Public::Call(const std::string &name, const std::vector<sol::object> &args)
{
    auto it = publics.find(name);
    if (it == publics.end())
        throw std::runtime_error("Public with name \"" + name + "\" does not exist");

    return it->second->ScriptFunction::Call(args);
}

void Public::DeleteAll()
{
    for (auto& kv : publics)
        delete kv.second;
    publics.clear();
}
