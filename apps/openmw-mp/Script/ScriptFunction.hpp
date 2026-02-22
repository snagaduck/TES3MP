#ifndef SCRIPTFUNCTION_HPP
#define SCRIPTFUNCTION_HPP

#include <string>
#include <vector>
#if defined(ENABLE_LUA)
#include <sol/sol.hpp>
struct lua_State;
#endif

class ScriptFunction
{
protected:
#if defined(ENABLE_LUA)
    lua_State* lua;
    std::string funcName;

    ScriptFunction(const std::string& funcName, lua_State* lua);
#endif
    ScriptFunction() = default;
    virtual ~ScriptFunction();

    sol::object Call(const std::vector<sol::object>& args);
};

#endif //SCRIPTFUNCTION_HPP
