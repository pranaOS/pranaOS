/**
 * @file noallocationguard.h
 * @author Krisna Pranav
 * @brief no allocation guard
 * @version 6.0
 * @date 2025-02-11
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/forward.h>
#include <mods/noncopyable.h>

#if defined(KERNEL)
#include <kernel/arch/processor.h>
#include <kernel/heap/kmalloc.h>
#else
#include <libc/mallocdefs.h>
#endif

namespace Mods
{

    class NoAllocationGuard
    {
        MOD_MAKE_NONCOPYABLE(NoAllocationGuard);
        MOD_MAKE_NONMOVABLE(NoAllocationGuard);

    public:
        /**
         * @brief Construct a new No Allocation Guard object
         * 
         */
        NoAllocationGuard()
            : m_allocation_enabled_previously(get_thread_allocation_state())
        {
            set_thread_allocation_state(false);
        }

        /**
         * @brief Destroy the No Allocation Guard object
         * 
         */
        ~NoAllocationGuard()
        {
            set_thread_allocation_state(m_allocation_enabled_previously);
        }

    private:
        /**
         * @brief Get the thread allocation state object
         * 
         * @return true 
         * @return false 
         */
        static bool get_thread_allocation_state()
        {
    #if defined(KERNEL)
            return Processor::current_thread()->get_allocation_enabled();
    #elif defined(__pranaos__)
            return s_allocation_enabled;
    #else
            return true;
    #endif
        }

        /**
         * @brief Set the thread allocation state object
         * 
         * @param value 
         */
        static void set_thread_allocation_state(bool value)
        {
    #if defined(KERNEL)
            Processor::current_thread()->set_allocation_enabled(value);
    #elif defined(__pranaos__)
            s_allocation_enabled = value;
    #else
            (void)value;
    #endif
        }

        bool m_allocation_enabled_previously{true};
    }; // class NoAllocationGuard
} // namespace Mods

using Mods::NoAllocationGuard;
