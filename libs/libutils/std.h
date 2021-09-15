#pragma once

#include <libutils/traits.h>

#if __CONFIG_IS_HOSTED__ == 0

inline void *operator new(size_t, void *ptr)
{
    return ptr;
}

inline void *operator new[](size_t, void *ptr)
{
    return ptr;
}

#else
#   include <new>
#endif

namespace Std
{

using nullptr_t = decltype(nullptr);

template <typename T>
struct initialize_list
{
private:
    T *_data;
    size_t size;   
 
};

}