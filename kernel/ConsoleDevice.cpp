/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Singleton.h>
#include <kernel/ConsoleDevice.h>
#include <kernel/IO.h>
#include <kernel/locking/SpinLock.h>
#include <kernel/Sections.h>
#include <kernel/kstdio.h>

#define CONSOLE_OUT_TO_BOCHS_DEBUG_PORT

static Singleton<ConsoleDevice> s_the;
static Kernel::SpinLock g_console_lock;

UNMAP_AFTER_INIT void ConsoleDevice::initialize()
{
    s_the.ensure_instance();
}

ConsoleDevice& ConsoleDevice::the()
{
    return *s_the;
}

bool ConsoleDevice::is_initialized()
{
    return s_the.is_initialized();
}

UNMAP_AFTER_INIT ConsoleDevice::ConsoleDevice()
    : CharacterDevice(5, 1)
{
}

UNMAP_AFTER_INIT ConsoleDevice::~ConsoleDevice()
{
}

bool ConsoleDevice::can_read(const Kernel::FileDescription&, size_t) const
{
    return false;
}

Kernel::KResultOr<size_t> ConsoleDevice::read(FileDescription&, u64, Kernel::UserOrKernelBuffer&, size_t)
{
    return 0;
}

Kernel::KResultOr<size_t> ConsoleDevice::write(FileDescription&, u64, const Kernel::UserOrKernelBuffer& data, size_t size)
{
    if (!size)
        return 0;

    return data.read_buffered<256>(size, [&](u8 const* bytes, size_t bytes_count) {
        for (size_t i = 0; i < bytes_count; i++)
            put_char((char)bytes[i]);
        return bytes_count;
    });
}

void ConsoleDevice::put_char(char ch)
{
    Kernel::ScopedSpinLock lock(g_console_lock);
#ifdef CONSOLE_OUT_TO_BOCHS_DEBUG_PORT
    IO::out8(IO::BOCHS_DEBUG_PORT, ch);
#endif
    m_logbuffer.enqueue(ch);
}
