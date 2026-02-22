#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/Net/RakNetManager.hpp>
#include <PacketPriority.h>
#include <RakPeer.h>
#include "BasePacket.hpp"

using namespace mwmp;

BasePacket::BasePacket(RakNet::RakPeerInterface *peer)
{
    packetID = 0;
    priority = HIGH_PRIORITY;
    reliability = RELIABLE_ORDERED;
    orderChannel = CHANNEL_SYSTEM;
    this->peer = peer;
}

void BasePacket::Packet(mwmp::NetBuffer *newBitstream, bool send)
{
    bs = newBitstream;
    packetValid = true;

    if (send)
    {
        bs->Write(packetID);
        bs->Write(guid);
    }
}

void BasePacket::SetReadStream(mwmp::NetBuffer *bitStream)
{
    bsRead = bitStream;
}

void BasePacket::SetSendStream(mwmp::NetBuffer *bitStream)
{
    bsSend = bitStream;
}

void BasePacket::SetStreams(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream)
{
    if (inStream != nullptr)
        bsRead = inStream;
    if (outStream != nullptr)
        bsSend = outStream;
}

uint32_t BasePacket::RequestData(mwmp::PlayerId targetGuid)
{
    bsSend->ResetWritePointer();
    bsSend->Write(packetID);
    bsSend->Write(targetGuid); // uint64_t; BitStream can write it directly
    RakNet::RakNetGUID rakGuid = mwmp::RakNetManager::getInstance()->ToGuid(targetGuid);
    return peer->Send(reinterpret_cast<const char*>(bsSend->GetData()),
                      static_cast<int>(bsSend->GetSize()),
                      HIGH_PRIORITY, RELIABLE_ORDERED, orderChannel, rakGuid, false);
}

uint32_t BasePacket::Send(RakNet::AddressOrGUID destination)
{
    bsSend->ResetWritePointer();
    Packet(bsSend, true);
    return peer->Send(reinterpret_cast<const char*>(bsSend->GetData()),
                      static_cast<int>(bsSend->GetSize()),
                      priority, reliability, orderChannel, destination, false);
}

uint32_t BasePacket::Send(mwmp::PlayerId target)
{
    RakNet::RakNetGUID rakGuid = mwmp::RakNetManager::getInstance()->ToGuid(target);
    return Send(RakNet::AddressOrGUID(rakGuid));
}

uint32_t BasePacket::Send(bool toOther)
{
    bsSend->ResetWritePointer();
    Packet(bsSend, true);
    RakNet::RakNetGUID rakGuid = mwmp::RakNetManager::getInstance()->ToGuid(guid);
    return peer->Send(reinterpret_cast<const char*>(bsSend->GetData()),
                      static_cast<int>(bsSend->GetSize()),
                      priority, reliability, orderChannel, rakGuid, toOther);
}

void BasePacket::Read()
{
    Packet(bsRead, false);
}

void BasePacket::setGUID(mwmp::PlayerId newGuid)
{
    guid = newGuid;
}

mwmp::PlayerId BasePacket::getGUID()
{
    return guid;
}
