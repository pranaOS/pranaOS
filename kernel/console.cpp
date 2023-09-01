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

/// @brief: initialize
void Console::initalize()
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
{}

/**
 * @return true 
 * @return false 
 */
bool Console::can_read(const Kernel::FileDescription&, size_t) const
{
    return false
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
    
    return (size_t)nread;
}