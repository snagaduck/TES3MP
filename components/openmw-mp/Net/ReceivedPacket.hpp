#ifndef OPENMW_RECEIVEDPACKET_HPP
#define OPENMW_RECEIVEDPACKET_HPP

#include <cstdint>
#include <string>

#include "PlayerId.hpp"
#include "NetBuffer.hpp"

namespace mwmp
{
    // Replaces RakNet::Packet* in processor dispatch.
    // Contains the full packet bytes (including header) plus pre-parsed fields.
    struct ReceivedPacket
    {
        PlayerId    sender;         // Replaces packet->guid
        std::string senderAddress;  // Replaces packet->systemAddress.ToString()
        NetBuffer   data;           // Full packet bytes including header
        uint8_t     packetId;       // data[0] — packet type identifier
    };
}

#endif // OPENMW_RECEIVEDPACKET_HPP
