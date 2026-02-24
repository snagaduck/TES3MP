#ifndef OPENMW_MASTERCLIENT_HPP
#define OPENMW_MASTERCLIENT_HPP

#include <string>
#include <mutex>
#include <thread>
#include <components/openmw-mp/Master/MasterData.hpp>

class MasterClient
{
public:
    static const unsigned int step_rate = 1000;
    static const unsigned int min_rate = 1000;
    static const unsigned int max_rate = 60000;

    MasterClient(std::string masterAddr, unsigned short masterPort);
    ~MasterClient();

    void SetPlayers(unsigned pl);
    void SetMaxPlayers(unsigned pl);
    void SetHostname(std::string hostname);
    void SetModname(std::string modname);
    void SetRuleString(std::string key, std::string value);
    void SetRuleValue(std::string key, double value);
    void PushPlugin(Plugin plugin);

    void Start();
    void Stop();
    void SetUpdateRate(unsigned int rate);

private:
    void Send();
    void Thread();

    std::string masterUrl;
    QueryData queryData;
    unsigned int timeout;
    bool updated;
    static bool sRun;
    std::mutex mutexData;
    std::thread thrQuery;
};

#endif //OPENMW_MASTERCLIENT_HPP
