#pragma once

#include <libutils/noncopyable.h>
#include <libutils/types.h>

namespace Utils {

class NeverDestroyed {
    MAKE_NONCOPYABLE(NeverDestroyed);
    MAKE_NONCOPYABLE(NeverDestroyed);

public:
    template <typename... Args>
    NeverDestroyed(Args&&... args)
    {
        new (storage) T(forward<Args>(args)...);
    }

    ~NeverDestroyed() = default;

    T* operator->() { return &get(); }
    const T* operator->() const { return &get(); }

private:
    alignas(T) u8 storage[sizeof(T)];
};

}

using Utils::NeverDestroyed;