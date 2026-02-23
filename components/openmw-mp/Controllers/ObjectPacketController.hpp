#ifndef OPENMW_OBJECTPACKETCONTROLLER_HPP
#define OPENMW_OBJECTPACKETCONTROLLER_HPP


#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/Object/ObjectPacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class ObjectPacketController
    {
    public:
        ObjectPacketController(mwmp::NetworkManager *network);
        ObjectPacket *GetPacket(unsigned char id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(unsigned char id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<ObjectPacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_OBJECTPACKETCONTROLLER_HPP
