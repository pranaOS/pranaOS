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

    Vector(std::initialize_list<T> data)
    {
        ensure_capacity(data.size());

        for (const auto &el : data)
        {
            push_back(el);
        }
    }

    Vector(AdoptTAg, T *storage, size_t size)
        : _storage(storage),
          _count(size),
          _capacity(size)
        
    {
    }

    ~Vector()
    {
        clear();
        if (_storage)
            free(_storage);
    }

    Vector &operator=(const Vector &other)
    {
        if (this != &other)
        {
            clear();

            ensure_capacity(other.count());
            _count = other.count();
            typed_copy(_storage, other._storage, _count);
        }
        return *this;
    }

    Vector &operator=(Vector &&other)
    {
        if (this != &other)
        {
            std::swap(_storage, other._storage);
            std::swap(_count, other._count);
            std::swap(_capacity, other._capacity);
        }

        return *this
    }

    bool operator!=(const Vector &other) const
    {
        return !(*this == other);
    }

    bool operator==(const Vector &other) const
    {
        if (this == &other)
        {
            return true;
        }

        if (_count != other._count)
        {
            return false;
        }

        for (size_t i = 0; i < _count; i++)
        {
            if(_storage[i] != other._storage[i])
            {
                return false;
            }
        }

        return true;
    }

    void clear()
    {
        if (!_storage)
        {
            return;
        }

        for (size_t i = 0; i < _count; i++)
        {
            _storage[i].~T();
        }

        _count = 0;
    }

};

}