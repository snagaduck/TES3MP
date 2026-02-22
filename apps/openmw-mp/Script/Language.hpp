#ifndef PLUGINSYSTEM3_LANGUAGE_HPP
#define PLUGINSYSTEM3_LANGUAGE_HPP

#include "Types.hpp"

#include <sol/sol.hpp>
#include <functional>
#include <vector>

struct lua_State;

class Language
{
public:
    virtual ~Language(){}
    virtual void LoadProgram(const char* filename) = 0;
    virtual int FreeProgram() = 0;
    virtual bool IsCallbackPresent(const char* name) = 0;
    virtual void Call(const char* name, std::function<void(lua_State*)> pushArgs, int nargs) = 0;
    virtual sol::object Call(const char* name, const std::vector<sol::object>& args) = 0;

    virtual lib_t GetInterface() = 0;

};


#endif //PLUGINSYSTEM3_LANGUAGE_HPP
