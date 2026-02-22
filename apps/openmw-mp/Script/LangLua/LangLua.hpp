#ifndef PLUGINSYSTEM3_LANGLUA_HPP
#define PLUGINSYSTEM3_LANGLUA_HPP

#include "lua.hpp"

#include <sol/sol.hpp>
#include <set>

#include "../Language.hpp"

class LangLua: public Language
{
public:
    virtual lib_t GetInterface() override;
    lua_State *lua;
public:
    LangLua();
    LangLua(lua_State *lua);
    ~LangLua();

    static void AddPackagePath(const std::string &path);
    static void AddPackageCPath(const std::string &path);

    static int MakePublic(lua_State *lua) noexcept;
    static int CallPublic(lua_State *lua);

    static int CreateTimer(lua_State *lua) noexcept;
    static int CreateTimerEx(lua_State *lua);

    virtual void LoadProgram(const char *filename) override;
    virtual int FreeProgram() override;
    virtual bool IsCallbackPresent(const char *name) override;
    virtual void Call(const char* name, std::function<void(lua_State*)> pushArgs, int nargs) override;
    virtual sol::object Call(const char* name, const std::vector<sol::object>& args) override;
private:
    static std::set<std::string> packageCPath;
    static std::set<std::string> packagePath;
};


#endif //PLUGINSYSTEM3_LANGLUA_HPP
