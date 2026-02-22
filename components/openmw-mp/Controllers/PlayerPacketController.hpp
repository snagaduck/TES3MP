#ifndef OPENMW_PLAYERPACKETCONTROLLER_HPP
#define OPENMW_PLAYERPACKETCONTROLLER_HPP


#include <RakPeerInterface.h>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/Player/PlayerPacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class PlayerPacketController
    {
    public:
        PlayerPacketController(RakNet::RakPeerInterface *peer);
        PlayerPacket *GetPacket(RakNet::MessageID id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(RakNet::MessageID id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<PlayerPacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_PLAYERPACKETCONTROLLER_HPP
