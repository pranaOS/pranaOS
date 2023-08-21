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
    struct HPETRegistersBlock;

    class HPET 
    {
    public:
        /**
         * @return true 
         * @return false 
         */
        static bool initialized();
        static bool test_and_initialize();
        static bool check_for_exisiting_periodic_timers();

        /**
         * @return HPET& 
         */
        static HPET& the();

        /**
         * @return u64 
         */
        u64 frequency() const;

        /**
         * @return const NonnullRefPtrVector<HPETComparator>& 
         */
        const NonnullRefPtrVector<HPETComparator>& comparators() const 
        { 
            return m_comparators; 
        }

        /// @brief: disable, enable
        void disable(const HPETComparator&);
        void enable(const HPETComparator&);

        void update_periodic_comparator_value();

        /**
         * @param comparator 
         */
        void update_non_periodic_comparator_value(const HPETComparator& comparator);

        /**
         * @param comparator_number 
         * @param irq_vector 
         */
        void set_comparator_irq_vector(u8 comparator_number, u8 irq_vector);

        /**
         * @param comparator 
         */
        void enable_periodic_interrupt(const HPETComparator& comparator);
        void disable_periodic_interrupt(const HPETComparator& comparator);

        /**
         * @param comparator_number 
         * @return Vector<unsigned> 
         */
        Vector<unsigned> capable_interrupt_numbers(u8 comparator_number);
        Vector<unsigned> capable_interrupt_numbers(const HPETComparator&);

    private:
        /**
         * @return const HPETRegistersBlock& 
         */
        const HPETRegistersBlock& registers() const;

        /**
         * @return HPETRegistersBlock& 
         */
        HPETRegistersBlock& registers();

        /// @brief: global[disable, enable]
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

        /**
         * @return u64 
         */
        u64 calculate_ticks_in_nanoseconds() const;
        
        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress find_acpi_hpet_registers_block();

        /**
         * @param acpi_hpet 
         */
        explicit HPET(PhysicalAddress acpi_hpet);
        
        PhysicalAddress m_physical_acpi_hpet_table;
        PhysicalAddress m_physical_acpi_hpet_registers;

        OwnPtr<Region> m_hpet_mmio_region;

        u64 m_main_counter_clock_period { 0 };

        u16 m_vendor_id;
        u16 m_minimum_tick;

        u64 m_frequency;

        u8 m_revision_id;

        bool counter_is_64_bit_capable : 1;
        bool legacy_replacement_route_capable : 1;

        NonnullRefPtrVector<HPETComparator> m_comparators;
    }; // class HPET

} // namespace Kernel