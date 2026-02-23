#ifndef OPENMW_SYSTEMPACKETCONTROLLER_HPP
#define OPENMW_SYSTEMPACKETCONTROLLER_HPP


#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/System/SystemPacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class SystemPacketController
    {
    public:
        SystemPacketController(mwmp::NetworkManager *network);
        SystemPacket *GetPacket(unsigned char id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(unsigned char id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<SystemPacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_SYSTEMPACKETCONTROLLER_HPP
