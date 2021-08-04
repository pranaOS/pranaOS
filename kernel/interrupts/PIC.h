/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/interrupts/IRQController.h>

namespace Kernel {

class PIC final : public IRQController {
public:
    PIC();
    virtual void enable(const GenericInterruptHandler&) override;
    virtual void disable(const GenericInterruptHandler&) override;
};

}
