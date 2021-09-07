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
    }
};

template <typename Type, typename... Args>
inline OwnPtr<Type> own(Args &&... args)
{
    return OwnPtr<Type>(new Type(std::forward<Args>(args)...));
}

}