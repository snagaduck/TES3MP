#include "MasterClient.hpp"
#include "Networking.hpp"
#include <components/openmw-mp/TimedLog.hpp>
#include <components/openmw-mp/Version.hpp>

#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cassert>
#include <chrono>
#include <sstream>
#include <thread>

using namespace mwmp;

bool MasterClient::sRun = false;

static std::string buildJson(const QueryData& qd, unsigned short gamePort)
{
    boost::property_tree::ptree pt;
    pt.put("hostname",    qd.GetName());
    pt.put("modname",     qd.GetGameMode());
    pt.put("version",     qd.GetVersion());
    pt.put("passw",       qd.GetPassword() != 0);
    pt.put("port",        gamePort);
    pt.put("players",     qd.GetPlayers());
    pt.put("max_players", qd.GetMaxPlayers());
    std::ostringstream ss;
    boost::property_tree::write_json(ss, pt, false);
    return ss.str();
}

static int httpPost(const std::string& url, const std::string& json)
{
    CURL* curl = curl_easy_init();
    if (!curl)
        return -1;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    // Suppress response output
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char*, size_t, size_t n, void*) { return n; });
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : -1;
}

MasterClient::MasterClient(std::string masterAddr, unsigned short masterPort)
    : masterUrl("http://" + masterAddr + ":" + std::to_string(masterPort))
    , timeout(15000)
    , updated(true)
{
}

MasterClient::~MasterClient()
{
    Stop();
}

void MasterClient::SetPlayers(unsigned pl)
{
    std::lock_guard<std::mutex> lock(mutexData);
    if ((unsigned)queryData.GetPlayers() != pl)
    {
        queryData.SetPlayers(pl);
        updated = true;
    }
}

void MasterClient::SetMaxPlayers(unsigned pl)
{
    std::lock_guard<std::mutex> lock(mutexData);
    if ((unsigned)queryData.GetMaxPlayers() != pl)
    {
        queryData.SetMaxPlayers(pl);
        updated = true;
    }
}

void MasterClient::SetHostname(std::string hostname)
{
    std::lock_guard<std::mutex> lock(mutexData);
    std::string substr = hostname.substr(0, 200);
    if (queryData.GetName() != substr)
    {
        queryData.SetName(substr.c_str());
        updated = true;
    }
}

void MasterClient::SetModname(std::string modname)
{
    std::lock_guard<std::mutex> lock(mutexData);
    std::string substr = modname.substr(0, 200);
    if (queryData.GetGameMode() != substr)
    {
        queryData.SetGameMode(substr.c_str());
        updated = true;
    }
}

void MasterClient::SetRuleString(std::string key, std::string value)
{
    std::lock_guard<std::mutex> lock(mutexData);
    if (queryData.rules.find(key) == queryData.rules.end()
        || queryData.rules[key].type != 's'
        || queryData.rules[key].str  != value)
    {
        ServerRule rule;
        rule.str  = value;
        rule.type = ServerRule::Type::string;
        queryData.rules.insert({key, rule});
        updated = true;
    }
}

void MasterClient::SetRuleValue(std::string key, double value)
{
    std::lock_guard<std::mutex> lock(mutexData);
    if (queryData.rules.find(key) == queryData.rules.end()
        || queryData.rules[key].type != 'v'
        || queryData.rules[key].val  != value)
    {
        ServerRule rule;
        rule.val  = value;
        rule.type = ServerRule::Type::number;
        queryData.rules.insert({key, rule});
        updated = true;
    }
}

void MasterClient::PushPlugin(Plugin plugin)
{
    std::lock_guard<std::mutex> lock(mutexData);
    queryData.plugins.push_back(plugin);
    updated = true;
}

void MasterClient::Send()
{
    unsigned short gamePort = Networking::get().getPort();
    std::string json;
    {
        std::lock_guard<std::mutex> lock(mutexData);
        json = buildJson(queryData, gamePort);
        updated = false;
    }

    std::string url = masterUrl + "/api/servers";
    if (httpPost(url, json) != 0)
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_WARN, "MasterClient: failed to reach master server at %s", url.c_str());
    else
        LOG_MESSAGE_SIMPLE(TimedLog::LOG_VERBOSE, "MasterClient: updated on master server");
}

void MasterClient::Thread()
{
    assert(!sRun);
    sRun = true;

    queryData.SetVersion(TES3MP_VERSION);
    queryData.SetPassword((int)Networking::get().isPassworded());

    auto* players = Players::getPlayers();
    while (sRun)
    {
        SetPlayers((int)players->size());

        // Rebuild player name list if it changed
        bool playerListChanged = (queryData.players.size() != players->size());
        if (!playerListChanged)
        {
            auto pIt = players->begin();
            for (size_t i = 0; i < queryData.players.size(); ++i, ++pIt)
            {
                if (queryData.players[i] != pIt->second->npc.mName)
                {
                    playerListChanged = true;
                    break;
                }
            }
        }

        if (playerListChanged)
        {
            std::lock_guard<std::mutex> lock(mutexData);
            queryData.players.clear();
            for (auto& p : *players)
            {
                if (!p.second->npc.mName.empty())
                    queryData.players.push_back(p.second->npc.mName);
            }
            updated = true;
        }

        Send();
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    }
}

void MasterClient::Start()
{
    thrQuery = std::thread(&MasterClient::Thread, this);
}

void MasterClient::Stop()
{
    if (!sRun)
        return;
    sRun = false;
    if (thrQuery.joinable())
        thrQuery.join();
}

void MasterClient::SetUpdateRate(unsigned int rate)
{
    if (rate < min_rate)
        rate = min_rate;
    else if (rate > max_rate)
        rate = max_rate;
    timeout = rate;
}
