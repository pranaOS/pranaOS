#pragma once

#include <libio/writer.h>
#include <libutils/minmax.h>
#include <libutils/slicestorage.h>
#include <libutils/stringstorage.h>

namespace IO
{

struct MemoryWriter : public Writer, public Seek
{
public:
    size_t _used = 0;
    size_t _size = 0;
    size_t _position = 0;
    uint8_t *_buffer = nullptr;

public:
    using Writer::flush;

    MemoryWriter() : MemoryWriter(16)
    {
    }

    MemoryWriter(size_t preallocated)
    {
        preallocated = MAX(preallocated, 16);

        _buffer = new uint8_t[preallocated];
        _buffer[0] = '\0';
        _size = preallocated;
        _used = 0;
        _position = 0;
    }

    ~MemoryWriter()
    {
        if (_buffer)
        {
            delete[] _buffer;
        }
    }

    ResultOr<size_t> length()
    {
        return _used;
    }

    ResultOr<size_t> tell()
    {
        return _position;
    }

    ResultOr<size_t> seek(SeekFrom from)
    {
        switch (from.whence)
        {
        case Whence::START:
            assert((size64_t)from.position <= _used);
            return _position = from.position;
        case Whence::CURRENT:
            assert(_position + from.position <= _used);
            return _position += from.position;
        default:
            ASSERT_NOT_REACHED();
        }
    }

    inline uint8_t *buffer()
    {
        return _buffer;
    }

    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        for (size_t i = 0; i < size; i++)
        {
            write(((uint8_t *)buffer)[i]);
        }
        return size;
    }

};

}