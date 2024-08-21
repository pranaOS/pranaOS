/**
 * @file irqcontroller.h
 * @author Krisna Pranav
 * @brief irqcontroller
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/types.h>

namespace Kernel 
{

    enum class IRQControllerType 
    {
        i8259 = 1,   
        i82093AA = 2 
    }; // enum

    class IRQController : public RefCounted<IRQController> 
    {
    public:
        /// @brief Destroy the IRQController object
        virtual ~IRQController() { }

        /// @brief: enable, disable
        virtual void enable(const GenericInterruptHandler&) = 0;
        virtual void disable(const GenericInterruptHandler&) = 0;

        /// @brief: hard_disable
        virtual void hard_disable() 
        { 
            m_hard_disabled = true; 
        }

        /**
         * @param number 
         * @return true 
         * @return false 
         */
        virtual bool is_vector_enabled(u8 number) const = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_enabled() const = 0;

        /**
         * @return true 
         * @return false 
         */
        bool is_hard_disabled() const 
        { 
            return m_hard_disabled; 
        }

        /// @brief: eoi
        virtual void eoi(const GenericInterruptHandler&) const = 0;

        /// @brief: spurious_eoi
        virtual void spurious_eoi(const GenericInterruptHandler&) const = 0;

        /**
         * @return size_t 
         */
        virtual size_t interrupt_vectors_count() const = 0;

        /**
         * @return u32 
         */
        virtual u32 gsi_base() const = 0;

        /**
         * @return u16 
         */
        virtual u16 get_isr() const = 0;
        virtual u16 get_irr() const = 0;

        /**
         * @return const char* 
         */
        virtual const char* model() const = 0;

        /**
         * @return IRQControllerType 
         */
        virtual IRQControllerType type() const = 0;

    protected:
        /// @brief Construct a new IRQController object
        IRQController() { }

        /// @breif: initialize
        virtual void initialize() = 0;

        bool m_hard_disabled { false };
    };

} // namespace Kernel
