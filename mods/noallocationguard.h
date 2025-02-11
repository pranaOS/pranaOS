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
#   include <kernel/arch/processor.h>
#else
#   include <libc/mallocdefs.h>
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
            : m_allocation_enabled_previously
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
        static bool get_thread_allocation_state()
        {
            return true;
        }

        /**
         * @brief Set the thread allocation state object
         * 
         * @param value 
         */
        static void set_thread_allocation_state(bool value)
        {
            (void)value;
        }

        bool m_allocation_enabled_previously { true };

    }; // class NoAllocationGuard
} // namespace Mods