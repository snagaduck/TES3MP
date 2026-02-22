#ifndef PLUGINSYSTEM3_SCRIPT_HPP
#define PLUGINSYSTEM3_SCRIPT_HPP

#include <boost/any.hpp>
#include <memory>
#include <vector>

#include "Types.hpp"
#include "ScriptFunction.hpp"
#include "ScriptFunctions.hpp"
#include "Language.hpp"

#include "Networking.hpp"

class Script : private ScriptFunctions
{
    // http://imgur.com/hU0N4EH
private:

    Language *lang;

    enum
    {
        SCRIPT_LUA
    };

    int script_type;

    typedef std::vector<std::unique_ptr<Script>> ScriptList;
    static ScriptList scripts;

    Script(const char *path);

    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;

protected:
    static std::string moddir;
public:
    ~Script();

    static void LoadScript(const char *script, const char* base);
    static void LoadScripts(char* scripts, const char* base);
    static void UnloadScripts();
    static void SetModDir(const std::string &moddir);
    static const char* GetModDir();

    template<typename... Args>
    static unsigned int Call(const char* name, Args&&... args) {
        unsigned int count = 0;

        for (auto& script : scripts)
        {
            if (!script->lang->IsCallbackPresent(name))
                continue;

#if defined (ENABLE_LUA)
            if (script->script_type == SCRIPT_LUA)
            {
                try
                {
                    script->lang->Call(name, [&](lua_State* L) {
                        (sol::stack::push(L, std::forward<Args>(args)), ...);
                    }, sizeof...(Args));
                }
                catch (std::exception &e)
                {
                    LOG_MESSAGE_SIMPLE(TimedLog::LOG_ERROR, e.what());
                    Script::Call("OnServerScriptCrash", e.what());

                    if (!mwmp::Networking::getPtr()->getScriptErrorIgnoringState())
                        throw;
                }
            }
#endif
            ++count;
        }

        return count;
    }
};

#endif //PLUGINSYSTEM3_SCRIPT_HPP
