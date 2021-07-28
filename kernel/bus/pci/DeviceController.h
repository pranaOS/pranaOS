/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/bus/pci/Definitions.h>

namespace Kernel {
class PCI::DeviceController {
public:
    Address pci_address() const { return m_address; }

virtual ~DeviceController() = default;
void enable_pin_based_interrupts() const;
void disable_pin_based_interrupt() const;

void enable_message_signalled_interrupts();
void disable_message_signalled_interrupts();

private:
    Address m_address;
};
}