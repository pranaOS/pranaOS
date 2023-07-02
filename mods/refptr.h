/**
 * @file refptr.h
 * @author Krisna Pranav
 * @brief RefPtr
 * @version 1.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "atomic.h"
#include "logstream.h"
#include "nonnull_refptr.h"
#include "stdlibextra.h"
#include "traits.h"
#include "types.h"

#ifdef KERNEL 
#   include <kernel/arch/i386/cpu.h>
#endif 

namespace Mods {
    template<typename T>
    class OwnPtr;

    template<typename T>
    struct RefPtrTraits {
        ALWAYS_INLINE static T* as_ptr(FlatPtr bits) {
            return (T*)(bits & ~(FlatPtr)1);
        }


    };
}

// using mods 
using Mods::RefPtr;
using Mods::OwnPtr;