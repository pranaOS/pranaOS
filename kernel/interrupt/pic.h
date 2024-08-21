/**
 * @file pic.h
 * @author Krisna Pranav
 * @brief PIC
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/interrupt/irqcontroller.h>

namespace Kernel 
{

    class PIC final : public IRQController 
    {
    public:

        /// @brief Construct a new PIC object
        PIC();

        /// @breif: enable + disable
        virtual void enable(const GenericInterruptHandler&) override;
        virtual void disable(const GenericInterruptHandler&) override;

        /// @breif: hard disable
        virtual void hard_disable() override;

        /// @breif: eoi
        virtual void eoi(const GenericInterruptHandler&) const override;

        /// @brief is_vector_enabled
        /// @param number 
        virtual bool is_vector_enabled(u8 number) const override;

        /// @breif: is_enabled ? 
        virtual bool is_enabled() const override;

        /// @brief: spurious_eoi
        virtual void spurious_eoi(const GenericInterruptHandler&) const override;

        /**
         * @return u16 
         */
        virtual u16 get_isr() const override;
        virtual u16 get_irr() const override;

        /**
         * @return u32 
         */
        virtual u32 gsi_base() const override 
        { 
            return 0; 
        }

        /**
         * @return size_t 
         */
        virtual size_t interrupt_vectors_count() const 
        { 
            return 16; 
        }

        /**
         * @return const char* 
         */
        virtual const char* model() const override 
        { 
            return "Dual i8259"; 
        }

        /**
         * @return IRQControllerType 
         */
        virtual IRQControllerType type() const override 
        { 
            return IRQControllerType::i8259; 
        }

    private:
        u16 m_cached_irq_mask { 0xffff };
        void eoi_interrupt(u8 irq) const;
        void enable_vector(u8 number);
        void remap(u8 offset);
        void complete_eoi() const;
        virtual void initialize() override;
    }; // class PIC

} // namespace Kernel
