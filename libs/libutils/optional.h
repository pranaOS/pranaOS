#pragma once

// includes
#include <assert.h>
#include <libutils/std.h>
#include <libutils/tags.h>

namespace Utils
{

template <typename T>
struct Optional
{
private:
    bool _present = false;
    T _storage;


public:
    ALWAYS_INLINE bool present() const
    {
        return _present;
    }

    ALWAYS_INLINE T &unwrap()
    {
        assert(present());
        return _storage;
    }

    ALWAYS_INLINE explict Optional()
    {
    }

    ALWAYS_INLINE Optional(const Optional &other)
    {
        if (other.present())
        {
            _present = true;
            new (&_storage) T(other.unwrap())
        }
    }

    ALWAYS_INLINE bool operator==(const T &other) const
    {
        if (!present())
        {
            return false
        }

        return unwrap()
    }

    ALWAYS_INLINE ~Optional()
    {
        clear();
    }

    ALWAYS_INLINE void clear()
    {
        if (_present)
        {
            _storage.~T();
            _present = false;
        }
    }

};


}