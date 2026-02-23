#ifndef OPENMW_WORLDSTATEPACKETCONTROLLER_HPP
#define OPENMW_WORLDSTATEPACKETCONTROLLER_HPP


#include <components/openmw-mp/Net/NetworkManager.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include "../Packets/Worldstate/WorldstatePacket.hpp"
#include <unordered_map>
#include <memory>

namespace mwmp
{
    class WorldstatePacketController
    {
    public:
        WorldstatePacketController(mwmp::NetworkManager *network);
        WorldstatePacket *GetPacket(unsigned char id);
        void SetStream(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);

        bool ContainsPacket(unsigned char id);

        typedef std::unordered_map<unsigned char, std::unique_ptr<WorldstatePacket> > packets_t;
    private:
        packets_t packets;
    };
}

#endif //OPENMW_WORLDSTATEPACKETCONTROLLER_HPP
