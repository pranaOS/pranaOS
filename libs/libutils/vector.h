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
};

}