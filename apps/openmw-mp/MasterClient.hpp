#ifndef OPENMW_MASTERCLIENT_HPP
#define OPENMW_MASTERCLIENT_HPP

#include <string>
#include <mutex>
#include <thread>
#include <components/openmw-mp/Master/MasterData.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include <components/openmw-mp/Net/PlayerId.hpp>
#include <components/openmw-mp/Net/RakNetManager.hpp>
#include <components/openmw-mp/Master/PacketMasterAnnounce.hpp>

class MasterClient
{
public:
    static const unsigned int step_rate = 1000;
    static const unsigned int min_rate = 1000;
    static const unsigned int max_rate = 60000;
public:
    MasterClient(mwmp::RakNetManager *rakNetManager, std::string queryAddr, unsigned short queryPort);
    void SetPlayers(unsigned pl);
    void SetMaxPlayers(unsigned pl);
    void SetHostname(std::string hostname);
    void SetModname(std::string hostname);
    void SetRuleString(std::string key, std::string value);
    void SetRuleValue(std::string key, double value);
    void PushPlugin(Plugin plugin);

    bool Process(RakNet::Packet *packet);
    void Start();
    void Stop();
    void SetUpdateRate(unsigned int rate);

private:
    void Send(mwmp::PacketMasterAnnounce::Func func);
    void Thread();
private:
    RakNet::SystemAddress masterServer;
    mwmp::RakNetManager *rakNetManager;
    mwmp::PlayerId masterServerId;
    QueryData queryData;
    unsigned int timeout;
    static bool sRun;
    std::mutex mutexData;
    std::thread thrQuery;
    mwmp::PacketMasterAnnounce pma;
    mwmp::NetBuffer writeStream;
    bool updated;
};


#endif //OPENMW_MASTERCLIENT_HPP
