/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Arch::x86
{

void reboot_8042();
void reboot_triple_fault();
void shutdown_virtual_machines();
void shutdown_acpi();

}