/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/acpi/Parser.h>
#include <kernel/filesystem/FileSystem.h>
#include <kernel/IO.h>
#include <kernel/Process.h>
#include <kernel/tty/ConsoleManagement.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$reboot()
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    if (!is_superuser())
        return EPERM;

    REQUIRE_NO_PROMISES;

    dbgln("acquiring FS locks...");
    FileSystem::lock_all();
    dbgln("syncing mounted filesystems...");
    FileSystem::sync();
    dbgln("attempting reboot via ACPI");
    if (ACPI::is_enabled())
        ACPI::Parser::the()->try_acpi_reboot();
    dbgln("attempting reboot via KB Controller...");
    IO::out8(0x64, 0xFE);

    return 0;
}

KResultOr<FlatPtr> Process::sys$halt()
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    if (!is_superuser())
        return EPERM;

    REQUIRE_NO_PROMISES;
    ConsoleManagement::the().switch_to_debug();

    dbgln("acquiring FS locks...");
    FileSystem::lock_all();
    dbgln("syncing mounted filesystems...");
    FileSystem::sync();
    dbgln("attempting system shutdown...");

    IO::out16(0x604, 0x2000);
    
    IO::out16(0x4004, 0x3400);
    
    IO::out16(0xb004, 0x2000);
    dbgln("shutdown attempts failed, applications will stop responding.");
    dmesgln("Shutdown can't be completed. It's safe to turn off the computer!");
    Processor::halt();
}

}
