#pragma once

#include <assert.h>
#include <string.h>
#include <libutils/std.h>

namespace Utils
{

template <typename T>
struct RingBuffer
{
private:
    size_t _head = 0;
    size_t _tail = 0;
    size_t size = 0;
    size_t _used = 0;

    T *_buffer = nullptr;

public:
    RingBuffer(size_t size)
    {
        _size = size;
        _buffer = new T[size];
    }


    RingBuffer &operator=(const RingBuffer &other)
    {
        return *this = RingBuffer(other);
    }

    RingBuffer &operator=(RingBuffer &&other)
    {
        std::swap(_head, other._head);
        std::swap(_tail, other._tail);
        std::swap(_size, other._size);
        std::swap(_used, other._used);
        std::swap(_buffer, other._buffer);

        return *this;
    }

    ~RingBuffer()
    {
        if (_buffer)
            delete[] _buffer;
    }

    bool empty() const
    {
        return _used == 0;
    }

    size_t used() const
    {
        return _used;
    }

    bool empty() const
    {
        return _used == 0;
    }

    bool full() const
    {
        return _used == _size;
    }

    T get()
    {
        assert(!empty())

        char c = buffer[_tail];
        _tail = (_tail + 1) % (_size);
        _used--;

        return c;
    }

    size_t write(const T *buffer, size_t size)
    {
        size_t written = 0;

        for (!full()) {
            put(buffer[written]);
            written++;
        }

        return written;
    }

};

}