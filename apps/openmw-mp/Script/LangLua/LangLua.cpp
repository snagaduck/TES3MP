#include <iostream>
#include "LangLua.hpp"
#include <Script/Script.hpp>

std::set<std::string> LangLua::packagePath;
std::set<std::string> LangLua::packageCPath;

void setLuaPath(lua_State* L, const char* path, bool cpath = false)
{
    std::string field = cpath ? "cpath" : "path";
    lua_getglobal(L, "package");

    lua_getfield(L, -1, field.c_str());
    std::string cur_path = lua_tostring(L, -1);
    cur_path.append(";");
    cur_path.append(path);
    lua_pop(L, 1);
    lua_pushstring(L, cur_path.c_str());
    lua_setfield(L, -2, field.c_str());
    lua_pop(L, 1);
}

lib_t LangLua::GetInterface()
{
    return reinterpret_cast<lib_t>(lua);
}

LangLua::LangLua(lua_State *lua)
{
    this->lua = lua;
}

LangLua::LangLua()
{
    lua = luaL_newstate();
    luaL_openlibs(lua); // load all lua std libs

    std::string p, cp;
    for (auto& path : packagePath)
        p += path + ';';

    for (auto& path : packageCPath)
        cp += path + ';';

    setLuaPath(lua, p.c_str());
    setLuaPath(lua, cp.c_str(), true);

}

LangLua::~LangLua()
{

}

void LangLua::LoadProgram(const char *filename)
{
    int err = 0;

    if ((err = luaL_loadfile(lua, filename)) != 0)
        throw std::runtime_error("Lua script " + std::string(filename) + " error (" + std::to_string(err) + "): \"" +
                            std::string(lua_tostring(lua, -1)) + "\"");

    sol::state_view sol_lua(lua);
    auto tes3mp = sol_lua["tes3mp"].get_or_create<sol::table>();

    tes3mp.set_function("CreateTimer", LangLua::CreateTimer);
    tes3mp.set_function("CreateTimerEx", LangLua::CreateTimerEx);
    tes3mp.set_function("MakePublic", LangLua::MakePublic);
    tes3mp.set_function("CallPublic", LangLua::CallPublic);
    tes3mp.set_function("StartTimer", &ScriptFunctions::StartTimer);
    tes3mp.set_function("StopTimer", &ScriptFunctions::StopTimer);
    tes3mp.set_function("RestartTimer", &ScriptFunctions::RestartTimer);
    tes3mp.set_function("FreeTimer", &ScriptFunctions::FreeTimer);
    tes3mp.set_function("IsTimerElapsed", &ScriptFunctions::IsTimerElapsed);

    ACTORAPI(tes3mp);
    BOOKAPI(tes3mp);
    CELLAPI(tes3mp);
    CHARCLASSAPI(tes3mp);
    CHATAPI(tes3mp);
    DIALOGUEAPI(tes3mp);
    FACTIONAPI(tes3mp);
    GUIAPI(tes3mp);
    ITEMAPI(tes3mp);
    MECHANICSAPI(tes3mp);
    MISCELLANEOUSAPI(tes3mp);
    OBJECTAPI(tes3mp);
    POSITIONAPI(tes3mp);
    QUESTAPI(tes3mp);
    RECORDSDYNAMICAPI(tes3mp);
    SHAPESHIFTAPI(tes3mp);
    SERVERAPI(tes3mp);
    SETTINGSAPI(tes3mp);
    SPELLAPI(tes3mp);
    STATAPI(tes3mp);
    WORLDSTATEAPI(tes3mp);

    if ((err = lua_pcall(lua, 0, 0, 0)) != 0)
        throw std::runtime_error("Lua script " + std::string(filename) + " error (" + std::to_string(err) + "): \"" +
                            std::string(lua_tostring(lua, -1)) + "\"");
}

int LangLua::FreeProgram()
{
    lua_close(lua);
    return 0;
}

bool LangLua::IsCallbackPresent(const char *name)
{
    lua_getglobal(lua, name);
    bool isFunc = lua_isfunction(lua, -1);
    lua_pop(lua, 1);
    return isFunc;
}

void LangLua::Call(const char* name, std::function<void(lua_State*)> pushArgs, int nargs)
{
    lua_getglobal(lua, name);
    pushArgs(lua);
    if (lua_pcall(lua, nargs, 0, 0) != 0)
    {
        std::string err = lua_tostring(lua, -1);
        lua_pop(lua, 1);
        throw std::runtime_error("Lua error: " + err);
    }
}

boost::any LangLua::Call(const char *name, const char *argl, const std::vector<boost::any> &args)
{
    int n_args = (int)(strlen(argl));

    lua_getglobal(lua, name);

    for (int index = 0; index < n_args; index++)
    {
        switch (argl[index])
        {
            case 'i':
                sol::stack::push(lua, boost::any_cast<unsigned int>(args.at(index)));
                break;

            case 'q':
                sol::stack::push(lua, boost::any_cast<signed int>(args.at(index)));
                break;

            case 'l':
                sol::stack::push(lua, boost::any_cast<unsigned long long>(args.at(index)));
                break;

            case 'w':
                sol::stack::push(lua, boost::any_cast<signed long long>(args.at(index)));
                break;

            case 'f':
                sol::stack::push(lua, boost::any_cast<double>(args.at(index)));
                break;

            case 'p':
                lua_pushlightuserdata(lua, boost::any_cast<void *>(args.at(index)));
                break;

            case 's':
                sol::stack::push(lua, boost::any_cast<const char *>(args.at(index)));
                break;

            case 'b':
                sol::stack::push(lua, boost::any_cast<int>(args.at(index)));
                break;
            default:
                throw std::runtime_error("Lua call: Unknown argument identifier " + argl[index]);
        }
    }

    if (lua_pcall(lua, n_args, 1, 0) != 0)
    {
        std::string err = lua_tostring(lua, -1);
        lua_pop(lua, 1);
        throw std::runtime_error("Lua error: " + err);
    }
    sol::object result = sol::stack::get<sol::object>(lua, -1);
    lua_pop(lua, 1);
    return boost::any(result);
}

void LangLua::AddPackagePath(const std::string& path)
{
    packagePath.emplace(path);
}

void LangLua::AddPackageCPath(const std::string& path)
{
    packageCPath.emplace(path);
}
