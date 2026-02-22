#ifndef OPENMW_ACTORPACKETCONTROLLER_HPP
#define OPENMW_ACTORPACKETCONTROLLER_HPP


#include <RakPeerInterface.h>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/Actor/ActorPacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class ActorPacketController
    {
    public:
        ActorPacketController(RakNet::RakPeerInterface *peer);
        ActorPacket *GetPacket(RakNet::MessageID id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(RakNet::MessageID id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<ActorPacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_ACTORPACKETCONTROLLER_HPP
