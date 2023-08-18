/**
 * @file genericinterrupthandler.h
 * @author Krisna Pranav
 * @brief generic interrupt handler
 * @version 1.0
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/hashtable.h>
#include <mods/string.h>
#include <mods/types.h>
#include <kernel/arch/i386/cpu.h>

namespace Kernel 
{

    enum class HandlerType : u8 
    {
        IRQHandler = 1,
        SharedIRQHandler = 2,
        UnhandledInterruptHandler = 3,
        SpuriousInterruptHandler = 4
    }; // enum

    class GenericInterruptHandler 
    {
    public:
        /**
         * @param interrupt_number 
         * @return GenericInterruptHandler& 
         */
        static GenericInterruptHandler& from(u8 interrupt_number);

        /// @brief Destroy the Generic Interrupt Handler object
        virtual ~GenericInterruptHandler();

        /**
         * @param regs 
         */
        virtual void handle_interrupt(const RegisterState& regs) = 0;

        /**
         * @return u8 
         */
        u8 interrupt_number() const 
        { 
            return m_interrupt_number; 
        }

        /**
         * @return size_t 
         */
        size_t get_invoking_count() const 
        { 
            return m_invoking_count.load(Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @return size_t 
         */
        virtual size_t sharing_devices_count() const = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_shared_handler() const = 0;
        virtual bool is_sharing_with_others() const = 0;

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const = 0;

        /**
         * @return const char* 
         */
        virtual const char* purpose() const = 0;
        virtual const char* controller() const = 0;

        virtual bool eoi() = 0;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void increment_invoking_counter()
        {
            m_invoking_count.fetch_add(1, Mods::MemoryOrder::memory_order_relaxed);
        }

    protected:

        /**
         * @param number 
         */
        void change_interrupt_number(u8 number);

        /**
         * @param interrupt_number 
         * @param disable_remap 
         */
        GenericInterruptHandler(u8 interrupt_number, bool disable_remap = false);

        /// @brief: disable_remap
        void disable_remap() 
        { 
            m_disable_remap = true; 
        }

    private:

        Atomic<u32> m_invoking_count { 0 };
        u8 m_interrupt_number { 0 };
        bool m_disable_remap { false };

    }; // class GenericInterruptHandler

} // namespace Kernel
