/**
 * @file genericinterrupthandler.h
 * @author Krisna Pranav
 * @brief generic interrupt handler
 * @version 6.0
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/intrusivelist.h>
#include <mods/types.h>
#include <kernel/arch/registerstate.h>

namespace Kernel 
{
    enum class HandlerType : u8 
    {
        IRQHandler = 1,
        SharedIRQHandler = 2,
        UnhandledInterruptHandler = 3,
        SpuriousInterruptHandler = 4
    }; // enum class HandlerType : u8

    class GenericInterruptHandler 
    {
    public:
        /**
         * @param interrupt_number 
         * @return GenericInterruptHandler& 
         */
        static GenericInterruptHandler& from(u8 interrupt_number);

        /**
         * @brief Destroy the Generic Interrupt Handler object
         * 
         */
        virtual ~GenericInterruptHandler()
        {
            VERIFY(!m_registered);
        }
        
        /**
         * @param regs 
         * @return true 
         * @return false 
         */
        virtual bool handle_interrupt(const RegisterState& regs) = 0;

        void will_be_destroyed();
        
        bool is_registered() const 
        { 
            return m_registered; 
        }

        void register_interrupt_handler();
        void unregister_interrupt_handler();

        /**
         * @return u8 
         */
        u8 interrupt_number() const 
        { 
            return m_interrupt_number; 
        }

        /**
         * @brief Get the invoking count object
         * 
         * @return size_t 
         */
        size_t get_invoking_count() const 
        { 
            return m_invoking_count; 
        }

        /**
         * @return size_t 
         */
        virtual size_t sharing_devices_count() const = 0;
        virtual bool is_shared_handler() const = 0;
        virtual bool is_sharing_with_others() const = 0;

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const = 0;
        virtual StringView purpose() const = 0;
        virtual StringView controller() const = 0;

        virtual bool eoi() = 0;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void increment_invoking_counter()
        {
            m_invoking_count++;
        }

    protected:
        /**
         * @param number 
         */
        void change_interrupt_number(u8 number);

        /**
         * @brief Construct a new Generic Interrupt Handler object
         * 
         * @param interrupt_number 
         * @param disable_remap 
         */
        GenericInterruptHandler(u8 interrupt_number, bool disable_remap = false);

        void disable_remap() { m_disable_remap = true; }

    private:
        Atomic<u32, Mods::MemoryOrder::memory_order_relaxed> m_invoking_count { 0 };
        u8 m_interrupt_number { 0 };
        bool m_disable_remap { false };
        bool m_registered { false };

        IntrusiveListNode<GenericInterruptHandler> m_list_node;

    public:
        using List = IntrusiveList<&GenericInterruptHandler::m_list_node>;
    }; // class GenericInterruptHandler 
} // namespace Kernel