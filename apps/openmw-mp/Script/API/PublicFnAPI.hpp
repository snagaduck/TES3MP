#ifndef PLUGINSYSTEM3_PUBLICFNAPI_HPP
#define PLUGINSYSTEM3_PUBLICFNAPI_HPP

#include <unordered_map>
#include <Script/ScriptFunction.hpp>


class Public : public ScriptFunction
{
private:
    ~Public();

    static std::unordered_map<std::string, Public *> publics;

#if defined(ENABLE_LUA)
    Public(const std::string& callback, lua_State *lua, const std::string &name);
#endif

public:
    template<typename... Args>
    static void MakePublic(Args &&... args)
    { new Public(std::forward<Args>(args)...); }

    static sol::object Call(const std::string &name, const std::vector<sol::object> &args);

    static void DeleteAll();
};

#endif //PLUGINSYSTEM3_PUBLICFNAPI_HPP
