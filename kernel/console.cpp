/**
 * @file console.cpp
 * @author Krisna Pranav
 * @brief console
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/console.h>
#include <kernel/io.h>
#include <kernel/spinlock.h>
#include <kernel/kstdio.h>
#include <mods/singleton.h>

#define CONSOLE_OUT_TO_E9

static Mods::Singleton<Console> s_the;

static Kernel::SpinLock g_console_lock;

/// @breif: initialize
void Console::initialize()
{
    s_the.ensure_instance();
}

/**
 * @return Console& 
 */
Console& Console::the()
{
    return *s_the;
}

/**
 * @return true 
 * @return false 
 */
bool Console::is_initialized()
{
    return s_the.is_initialized();
}

/// @brief Construct a new Console::Console object
Console::Console()
    : CharacterDevice(5, 1)
{
}

/// @brief Destroy the Console::Console object
Console::~Console()
{
}

/**
 * @return true 
 * @return false 
 */
bool Console::can_read(const Kernel::FileDescription&, size_t) const
{
    return false;
}

/**
 * @return Kernel::KResultOr<size_t> 
 */
Kernel::KResultOr<size_t> Console::read(Kernel::FileDescription&, size_t, Kernel::UserOrKernelBuffer&, size_t)
{
    return 0;
}

/**
 * @param data 
 * @param size 
 * @return Kernel::KResultOr<size_t> 
 */
Kernel::KResultOr<size_t> Console::write(Kernel::FileDescription&, size_t, const Kernel::UserOrKernelBuffer& data, size_t size)
{
    if (!size)
        return 0;

    ssize_t nread = data.read_buffered<256>(size, [&](const u8* bytes, size_t bytes_count) {
        for (size_t i = 0; i < bytes_count; i++)
            put_char((char)bytes[i]);
        return (ssize_t)bytes_count;
    });

    if (nread < 0)
        return Kernel::KResult(nread);

    return (size_t)nread;
}

/**
 * @param ch 
 */
void Console::put_char(char ch)
{
    Kernel::ScopedSpinLock lock(g_console_lock);

#ifdef CONSOLE_OUT_TO_E9
    IO::out8(0xe9, ch);
#endif
    m_logbuffer.enqueue(ch);
}
