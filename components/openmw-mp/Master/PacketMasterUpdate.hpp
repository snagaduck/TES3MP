#ifndef OPENMW_PACKETMASTERUPDATE_HPP
#define OPENMW_PACKETMASTERUPDATE_HPP

#include "../Packets/BasePacket.hpp"
#include "MasterData.hpp"
#include <RakNetTypes.h>

namespace mwmp
{
    class ProxyMasterPacket;
    class PacketMasterUpdate : public BasePacket
    {
        friend class ProxyMasterPacket;
    public:
        explicit PacketMasterUpdate(mwmp::NetworkManager *network);

        void Packet(mwmp::NetBuffer *newBitstream, bool send) override;

        void SetServer(std::pair<RakNet::SystemAddress, QueryData> *serverPair);
    private:
        std::pair<RakNet::SystemAddress, QueryData> *server;
    };
}

#endif //OPENMW_PACKETMASTERUPDATE_HPP
