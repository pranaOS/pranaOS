/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <kernel/ACPI/Parser.h>
#include <kernel/Interrupts/IRQHandler.h>
#include <kernel/Mutex.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/VM/PhysicalPage.h>

namespace Kernel::ACPI {

class DynamicParser final
    : public IRQHandler
    , public Parser {
    friend class Parser;
};

}