/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/Types.h>

namespace Kernel {

enum class IRQControllerType {
    i8259 = 1,  
    i82093AA = 2
};

class IRQController : public RefCounted<IRQController> {
public:
    virtual ~IRQController() = default;

    virtual void enable(const GenericInterruptHandler&) = 0;
    virtual void disable(const GenericInterruptHandler&) = 0;
    virtual void hard_disable() { m_hard_disabled = true; }
    virtual bool is_vector_enabled(u8 number) const = 0;
    virtual bool is_enabled() const = 0;
    bool is_hard_disabled() const { return m_hard_disabled; }
    virtual void eoi(const GenericInterruptHandler&) const = 0;
    virtual void spurious_eoi(const GenericInterruptHandler&) const = 0;
    virtual size_t interrupt_vectors_count() const = 0;
    virtual u32 gsi_base() const = 0;
    virtual u16 get_isr() const = 0;
    virtual u16 get_irr() const = 0;
    virtual StringView model() const = 0;
    virtual IRQControllerType type() const = 0;

protected:
    IRQController() = default;
    virtual void initialize() = 0;
    bool m_hard_disabled { false };
};
}
