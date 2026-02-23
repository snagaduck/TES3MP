#ifndef OPENMW_PLAYERPACKETCONTROLLER_HPP
#define OPENMW_PLAYERPACKETCONTROLLER_HPP


#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/Player/PlayerPacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class PlayerPacketController
    {
    public:
        PlayerPacketController(mwmp::NetworkManager *network);
        PlayerPacket *GetPacket(unsigned char id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(unsigned char id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<PlayerPacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_PLAYERPACKETCONTROLLER_HPP
