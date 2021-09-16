#pragma once

#include <string.h>
#include <libutils/minmax.h>
#include <libutils/iteration.h>
#include <libutils/refptr.h>
#include <libutils/std.h>

namespace Utils
{

template <typename T>
void typed_copy(T *destination, T *source, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        new (&destination[i]) T(source[i]);
    }
}

template <typename T>
void typed_move(T *destination, T *source, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (destination <= source)
        {
            new (&destination[i]) T(std::move(source[i]));
        }
        else
        {
            new (&destination[count - i - 1]) T(std::move(source[count - i - 1]));
        }
    }
}


template <typename T>
struct Vector
{
private:
    T *_storage = nullptr;
    size_t _count = 0;
    size_t _capacity = 0;

public:
    size_t count() const
    {
        return _count;
    }

    bool empty() const
    {
        return _count == 0;
    }

    bool any() const
    {
        return !empty();
    }

    T &at(size_t index)
    {
        assert(index < _count);
        return _storage[index];
    }

    const T &at(size_t index) const
    {
        assert(index < _count);
        return _storage[index];
    }

    Vector() : Vector(16) {}

    Vector(size_t capacity)
    {
        ensure_capacity(capacity);
    }
};

}