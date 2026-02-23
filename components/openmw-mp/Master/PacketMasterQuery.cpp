#include <components/openmw-mp/NetworkMessages.hpp>
#include <iostream>
#include "MasterData.hpp"
#include "PacketMasterQuery.hpp"
#include "ProxyMasterPacket.hpp"


using namespace mwmp;
using namespace RakNet;

PacketMasterQuery::PacketMasterQuery(mwmp::NetworkManager *network) : BasePacket(network)
{
    packetID = ID_MASTER_QUERY;
    orderChannel = CHANNEL_MASTER;
    reliable = true;
}

void PacketMasterQuery::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    bs = newBitstream;
    if (send)
        bs->Write(packetID);

    int32_t serversCount = servers->size();

    RW(serversCount, send);

    std::map<SystemAddress, QueryData>::iterator serverIt;
    if (send)
        serverIt = servers->begin();

    QueryData server;
    std::string addr;
    uint16_t port;
    while (serversCount--)
    {
        if (send)
        {
            addr = serverIt->first.ToString(false);
            port = serverIt->first.GetPort();
            server = serverIt->second;
        }
        RW(addr, send);
        RW(port, send);

        ProxyMasterPacket::addServer(this, server, send);

        if(addr.empty())
        {
            std::cerr << "Address empty. Aborting PacketMasterQuery::Packet" << std::endl;
            return;
        }

        if (send)
            serverIt++;
        else
            servers->insert(std::pair<SystemAddress, QueryData>(SystemAddress(addr.c_str(), port), server));
    }

}

void PacketMasterQuery::SetServers(std::map<SystemAddress, QueryData> *serverMap)
{
    servers = serverMap;
}
