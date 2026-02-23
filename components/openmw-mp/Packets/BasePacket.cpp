#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/Net/NetworkManager.hpp>
#include "BasePacket.hpp"

using namespace mwmp;

BasePacket::BasePacket(mwmp::NetworkManager *network)
{
    packetID = 0;
    reliable = true;
    orderChannel = CHANNEL_SYSTEM;
    this->network = network;
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
    bsSend->Write(targetGuid);
    network->Send(targetGuid, *bsSend, true, orderChannel);
    return 1;
}

uint32_t BasePacket::Send(mwmp::PlayerId target)
{
    bsSend->ResetWritePointer();
    Packet(bsSend, true);
    network->Send(target, *bsSend, reliable, orderChannel);
    return 1;
}

uint32_t BasePacket::Send(bool toOther)
{
    bsSend->ResetWritePointer();
    Packet(bsSend, true);
    if (toOther)
        network->Broadcast(*bsSend, guid, reliable, orderChannel);
    else
        network->Send(guid, *bsSend, reliable, orderChannel);
    return 1;
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
