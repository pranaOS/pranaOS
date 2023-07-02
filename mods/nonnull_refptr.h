/**
 * @file nonnull_refptr.h
 * @author Krisna Pranav
 * @brief nonnullrefptr
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "atomic.h"
#include "logstream.h"
#include "stdlibextra.h"
#include "types.h"
#ifdef KERNEL
#   include <kernel/arch/i386/cpu.h>
#endif

namespace Mods {
    template<typename T>
    class OwnPtr;

    template<typename T, typename PtrTraits>
    class RefPtr;

    template<typename T>
    ALWAYS_INLINE void ref_if_not_null(T* ptr) {
        if(ptr)
            ptr->ref();
    }

    template<typename T>
    ALWAYS_INLINE void unref_if_not_null(T* ptr) {
        if (ptr)
            ptr->unref();
    }

    template<typename T>    
    class NonnullRefPtr {
        template<typename U, typename P>
        friend class RefPtr;

        template<typename U>
        class NonnullRefPtr;

        template<typename U>
        class WekPtr;

    public:
        using ElementType = T;

    private:
        NonnullRefPtr() = delete;

        ALWAYS_INLINE T* as_ptr() const {
            return (T*)(m_bits.load(Mods::MemoryOrder::memory_order_acq_rel));
        }
    };
}
