/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/RefPtr.h>
#include <base/String.h>
#include <base/Types.h>
#include <kernel/interrupts/GenericInterruptHandler.h>
#include <kernel/interrupts/IRQController.h>

namespace Kernel {

class IRQHandler : public GenericInterruptHandler {
public:
    virtual ~IRQHandler();

    virtual bool handle_interrupt(const RegisterState& regs) override { return handle_irq(regs); }
    virtual bool handle_irq(const RegisterState&) = 0;

    void enable_irq();
    void disable_irq();

    virtual bool eoi() override;

    virtual HandlerType type() const override { return HandlerType::IRQHandler; }
    virtual StringView purpose() const override { return "IRQ Handler"; }
    virtual StringView controller() const override { return m_responsible_irq_controller->model(); }

    virtual size_t sharing_devices_count() const override { return 0; }
    virtual bool is_shared_handler() const override { return false; }
    virtual bool is_sharing_with_others() const override { return m_shared_with_others; }

protected:
    void change_irq_number(u8 irq);
    explicit IRQHandler(u8 irq);

private:
    bool m_shared_with_others { false };
    bool m_enabled { false };
    RefPtr<IRQController> m_responsible_irq_controller;
};

}
