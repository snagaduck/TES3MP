#ifndef OPENMW_PACKETMASTERQUERY_HPP
#define OPENMW_PACKETMASTERQUERY_HPP

#include "../Packets/BasePacket.hpp"
#include "MasterData.hpp"
#include <RakNetTypes.h>

namespace mwmp
{
    class ProxyMasterPacket;
    class PacketMasterQuery : public BasePacket
    {
        friend class ProxyMasterPacket;
    public:
        explicit PacketMasterQuery(mwmp::NetworkManager *network);

        void Packet(mwmp::NetBuffer *newBitstream, bool send) override;

        void SetServers(std::map<RakNet::SystemAddress, QueryData> *serverMap);
    private:
        std::map<RakNet::SystemAddress, QueryData> *servers;
    };
}

#endif //OPENMW_PACKETMASTERQUERY_HPP
