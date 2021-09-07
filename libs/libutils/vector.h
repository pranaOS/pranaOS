#pragam once

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
struct Vector
{
private:
    T *_storage = nullptr;
    size_t _count = 0;
    size_t _capacity = 0;

public:
    Vector() : Vector(16) {}

    Vector(size_t capacity)
    {
        ensure_capacity(capacity)
    }

    void ensure_capacity(size_t capacity)
    {
        capacity = MAX(capacity, 16);

        if (!_storage)
        {
            _storage = reinterpret_cast<T *>(calloc(capacity, sizeof(T)));
            _capacity = capacity;
            _count = 0;

            return;
        }

        capacity = MAX(_count, capacity);

        if (capacity <= _capacity)
        {
            return;
        }

        size_t new_capacity = _capacity;

        while (new_capacity <= capacity)
        {
            new_capacity += new_capacity / 4;
        }

        T *new_storage = reinterpret_cast<T *>(calloc(new_capacity, sizeof(T)));

        for (size_t i = 0; i < _count; i++)
        {
            new (&new_storage[i]) T(std::move(_storage[i]));
        }

        free(_storage);
        _storage = new_storage;
        _capacity = new_capacity;
    }

    ~Vector()
    {
        clear();

        if (_storage)
            free(_storage);
    }

    T &operator[](size_t index)
    {
        assert(index < _count);
        return _storage[index];
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
            if (_storage[i] != other._storage[i])
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

    T pop_back()
    {
        assert(_count > 0);

        T value = std::move(_storage[_count - 1]);
        remove_index(_count - 1);

        return value;
    }

    T &peek() const
    {
        assert(_count > 0);
        return _storage[0];
    }

    T &peek_back() const
    {
        assert(_count > 0);
        return _storage[_count - 1];
    }

    bool contains(const T &value) const
    {
        for (size_t i = 0; i < _count; i++)
        {
            if (_storage[i] == value)
            {
                return true;
            }
        }

        return false;
    }

    ContiguousIterator<T> begin() const { return _storage; }
    ContiguousIterator<T> end() const { return _storage + _count; }
};

}