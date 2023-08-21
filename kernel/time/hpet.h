/**
 * @file hpet.h
 * @author Krisna Pranav
 * @brief hpet
 * @version 6.0
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptrvector.h>
#include <mods/ownptr.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <kernel/physical_address.h>
#include <kernel/vm/region.h>

namespace Kernel
{

    class HPETComparator;
    struct HPETRegisterBlock;

    class HPET
    {
    private:
        const HPETRegisterBlock& registers() const;
        HPETRegisterBlock& registers();

        void global_disable();
        void global_enable();

        /**
         * @param comparator_number 
         * @return true 
         * @return false 
         */
        bool is_periodic_capable(u8 comparator_number) const;

        /**
         * @param timers_count 
         */
        void set_comparators_to_optimal_interrupt_state(size_t timers_count);

        u64 m_main_counter_clock_period { 0 };
        u16 m_vendor_id;
        u16 m_minimum_tick;
        
    }; // class HPET

} // namespace Kernel