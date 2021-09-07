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

    template <typename U>
    OwnPtr &operator=(OwnPtr<U> &&other)
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

    T *operator->() const
    {
        assert(_ptr);
        return _ptr;
    }

    operator bool() const
    {
        return _ptr != nullptr;
    }

    bool operator() const
    {
        return _ptr == nullptr;
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
inline OwnPtr<Type> own(Args &&... args)
{
    return OwnPtr<Type>(new Type(std::forward<Args>(args)...));
}

}