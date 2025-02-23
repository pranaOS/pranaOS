/**
 * @file regs.h
 * @author Krisna Pranav
 * @brief registers 
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

#if defined(__cplusplus) && defined(__cpp_concepts)
#    include <mods/ypes.h>
#else
#    include <sys/types.h>
#endif

#include <kernel/arch/mcontext.h>

#ifdef __cplusplus
struct [[gnu::packed]] PtraceRegisters : public __mcontext {
#    if defined(__cplusplus) && defined(__cpp_concepts)
    FlatPtr ip() const
    {
#        if ARCH(I386)
        return eip;
#        else
        return rip;
#        endif
    }

    void set_ip(FlatPtr ip)
    {
#        if ARCH(I386)
        eip = ip;
#        else
        rip = ip;
#        endif
    }

    FlatPtr bp() const
    {
#        if ARCH(I386)
        return ebp;
#        else
        return rbp;
#        endif
    }

    void set_bp(FlatPtr bp)
    {
#        if ARCH(I386)
        ebp = bp;
#        else
        rbp = bp;
#        endif
    }
#    endif
};

#else
typedef struct __mcontext PthreadRegisters;
#endif
