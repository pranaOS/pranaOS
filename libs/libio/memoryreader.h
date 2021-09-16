#pragma once

#include <libio/reader.h>
#include <libutils/minmax.h>
#include <libutils/slice.h>

namespace IO
{

struct MemoryReader final : public Reader, public Seek
{
private:
    Slice _memory;
    size_t _position = 0;

public:
    Slice memory() 
    {
        return _memory;
    }

    MemoryReader(const char *cstring)
        : _memory{cstring}
    {
    }

    MemoryReader(const void *ptr, size_t size)
        : _memory(ptr, size)
    {
    }

    MemoryReader(Slice memory)
        : _memory{memory}
    {
    }

    MemoryReader(RawStorage &raw)
        : _memory(raw.storage())
    {
    }

    ResultOr<size_t> read(void *buffer, size_t size) override
    {
        if (_position > _memory.size())
        {
            return 0;
        }
    }

};

}