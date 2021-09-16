#pragma once

#include <assert.h>
#include <libutils/std.h>

namespace Utils
{

template <typename T>
struct OwnPtr
{
private:
    T *_ptr = nullptr;

public:
    OwnPtr() {}
    OwnPtr(nullptr) {}

    OwnPtr(T *ptr) : _ptr(ptr) {}

    OwnPtr(const OwnPtr &other) : _ptr(const_cast<OwnPtr &>(other).give_ref()) {}

    ~OwnPtr()
    {
        if (_ptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    OwnPtr &operator=(OwnPtr &other)
    {
        if (naked() != other.naked())
        {
            if (_ptr)
            {
                delete _ptr;
            }
            _ptr = other.give_ref();
        }

        return *this;
    }

    bool operator==(T *other) const
    {
        return _ptr == other;
    }

    operator bool() const
    {
        return _ptr != nullptr;
    }

    [[nodiscard]] T *give_ref()
    {
        auto ref = _ptr;
        _ptr = nullptr;

        return ref;
    }

    T *naked()
    {
        return _ptr;
    }

};

template <typename Type, typename... Args> 
inline OwnPtr<Type> own(Args &&...args)
{
    return OwnPtr<Type>(new Type(std::forward<Args>(args)...));
}

}