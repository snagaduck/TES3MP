#ifndef OPENMW_NETBUFFER_HPP
#define OPENMW_NETBUFFER_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "PlayerId.hpp"

namespace mwmp
{
    // Replacement for RakNet::BitStream.
    // Byte-oriented (no bit packing), little-endian, length-prefixed strings.
    // Compression is accepted in the API but treated as a no-op (protocol break vs RakNet).
    class NetBuffer
    {
    public:
        // Write mode — starts empty.
        NetBuffer() : readPos(0) {}

        // Read mode — wraps existing bytes (does NOT take ownership).
        NetBuffer(const void* data, size_t len)
            : buf(static_cast<const uint8_t*>(data),
                  static_cast<const uint8_t*>(data) + len)
            , readPos(0)
        {}

        // --- Write helpers ---

        template<typename T>
        void Write(T value)
        {
            static_assert(std::is_trivially_copyable<T>::value, "NetBuffer::Write requires a trivially copyable type");
            const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
            buf.insert(buf.end(), p, p + sizeof(T));
        }

        template<typename T>
        void WriteCompressed(T value)
        {
            // Compression dropped — protocol break from RakNet is acceptable.
            Write(value);
        }

        void Write(bool value)
        {
            buf.push_back(value ? 1u : 0u);
        }

        void Write(const std::string& str)
        {
            uint16_t len = static_cast<uint16_t>(str.size() < 0xFFFF ? str.size() : 0xFFFF);
            Write(len);
            buf.insert(buf.end(),
                       reinterpret_cast<const uint8_t*>(str.data()),
                       reinterpret_cast<const uint8_t*>(str.data()) + len);
        }

        // Write with an explicit byte count (mirrors RakNet BitStream::Write(data, bits)).
        // Here `size` is treated as byte count to keep it simple.
        template<typename T>
        void Write(T& data, uint32_t byteCount)
        {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(&data);
            buf.insert(buf.end(), p, p + byteCount);
        }

        // --- Read helpers (return false on underflow) ---

        template<typename T>
        bool Read(T& value)
        {
            static_assert(std::is_trivially_copyable<T>::value, "NetBuffer::Read requires a trivially copyable type");
            if (readPos + sizeof(T) > buf.size())
                return false;
            std::memcpy(&value, buf.data() + readPos, sizeof(T));
            readPos += sizeof(T);
            return true;
        }

        template<typename T>
        bool ReadCompressed(T& value)
        {
            return Read(value);
        }

        bool Read(bool& value)
        {
            if (readPos >= buf.size())
                return false;
            value = (buf[readPos++] != 0);
            return true;
        }

        bool Read(std::string& str)
        {
            uint16_t len = 0;
            if (!Read(len))
                return false;
            if (readPos + len > buf.size())
                return false;
            str.assign(reinterpret_cast<const char*>(buf.data() + readPos), len);
            readPos += len;
            return true;
        }

        // Read with explicit byte count.
        template<typename T>
        bool Read(T& data, uint32_t byteCount)
        {
            if (readPos + byteCount > buf.size())
                return false;
            std::memcpy(&data, buf.data() + readPos, byteCount);
            readPos += byteCount;
            return true;
        }

        // --- Pointer resets ---

        void ResetWritePointer()
        {
            buf.clear();
            readPos = 0;
        }

        void ResetReadPos()
        {
            readPos = 0;
        }

        // --- Accessors ---

        const uint8_t* GetData() const { return buf.data(); }
        size_t         GetSize() const { return buf.size(); }

        // Header layout: 1 byte packetID + sizeof(PlayerId) bytes GUID == 9 bytes,
        // same as the old RakNet layout.
        static constexpr size_t headerSize()
        {
            return 1 + sizeof(PlayerId);
        }

    private:
        std::vector<uint8_t> buf;
        size_t               readPos;
    };
}

#endif // OPENMW_NETBUFFER_HPP
