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
    };

    class GenericInterruptHandler
    {
    public:
        static GenericInterruptHandler& from(u8 interrupt_number);
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

    protected:
        void change_interrupt_number(u8 number);

        GenericInterruptHandler(u8 interrupt_number, bool disable_remap = false);

        void disbale_remap()
        {
            m_disable_remap = true;
        }

    private:
        Atomic<u32> m_invoking_count { 0 };
        u8 m_interrupt_number { 0 };
        bool m_disable_remap { false };
    }; // class 
} // namespace Kernel   