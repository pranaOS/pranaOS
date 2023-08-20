/**
 * @file pic.h
 * @author Krisna Pranav
 * @brief PIC
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        PIC();

        virtual void enable(const GenericInterruptHandler&) override;

    private:
        u16 m_cached_irq_mask { 0xffff };
        void eoi_interrupt(u8 irq) const;
        void enable_vector(u8 number);
        void remap(u8 offset);
        void complete_eoi() const;
        virtual void initialize() override;
    }; // class PIC 

} // namespace Kernel