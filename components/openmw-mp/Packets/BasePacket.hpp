#ifndef OPENMW_BASEPACKET_HPP
#define OPENMW_BASEPACKET_HPP

#include <string>
#include <components/openmw-mp/Net/PlayerId.hpp>
#include <components/openmw-mp/Net/NetBuffer.hpp>
#include <components/openmw-mp/Net/NetworkManager.hpp>


namespace mwmp
{
    class BasePacket
    {
    public:
        explicit BasePacket(mwmp::NetworkManager *network);

        virtual ~BasePacket() = default;

        virtual void Packet(mwmp::NetBuffer *newBitstream, bool send);
        virtual uint32_t Send(bool toOtherPlayers = true);
        virtual uint32_t Send(mwmp::PlayerId target);
        virtual void Read();

        void setGUID(mwmp::PlayerId newGuid);
        mwmp::PlayerId getGUID();

        void SetReadStream(mwmp::NetBuffer *bitStream);
        void SetSendStream(mwmp::NetBuffer *bitStream);
        void SetStreams(mwmp::NetBuffer *inStream, mwmp::NetBuffer *outStream);
        virtual uint32_t RequestData(mwmp::PlayerId targetGuid);

        static inline uint32_t headerSize()
        {
            return static_cast<uint32_t>(1 + sizeof(mwmp::PlayerId)); // packetID + PlayerId (uint64_t)
        }

        uint8_t GetPacketID() const
        {
            return packetID;
        }

        bool isPacketValid() const
        {
            return packetValid;
        }

    protected:
        // RW with explicit byte count
        template<class templateType>
        bool RW(templateType &data, uint32_t size, bool write)
        {
            if (write)
                bs->Write(data, size);
            else
                return bs->Read(data, size);
            return true;
        }

        // RW with optional compression (compression is a no-op in NetBuffer)
        template<class templateType>
        bool RW(templateType &data, bool write, bool compress = 0)
        {
            if (write)
            {
                if (compress)
                    bs->WriteCompressed(data);
                else
                    bs->Write(data);
                return true;
            }
            else
            {
                if (compress)
                    return bs->ReadCompressed(data);
                else
                    return bs->Read(data);
            }
        }

        bool RW(bool &data, bool write)
        {
            if (write)
                bs->Write(data);
            else
                return bs->Read(data);
            return true;
        }

        const static uint32_t maxStrSize = 64 * 1024; // 64 KiB

        bool RW(std::string &str, bool write, bool compress = false, std::string::size_type maxSize = maxStrSize)
        {
            // RakString + compression removed — protocol break from RakNet.
            // NetBuffer uses a length-prefixed UTF-8 string format.
            if (write)
            {
                std::string capped = str.size() > maxSize ? str.substr(0, maxSize) : str;
                bs->Write(capped);
                return true;
            }
            else
            {
                bool res = bs->Read(str);
                if (res && str.size() > maxSize)
                    str.resize(maxSize);
                return res;
            }
        }

    protected:
        uint8_t packetID;
        bool reliable;
        int8_t orderChannel;
        mwmp::NetBuffer *bsRead, *bsSend, *bs;
        mwmp::NetworkManager *network;
        mwmp::PlayerId guid;
        bool packetValid;
    };
}

#endif //OPENMW_BASEPACKET_HPP
