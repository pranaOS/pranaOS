/**
 * @file ptymultiplexer.cpp
 * @author Krisna Pranav
 * @brief pty multi plexer
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/singleton.h>
#include <kernel/filesystem/filedescription.h>
#include <kernel/process.h>
#include <libc/errno_codes.h>
#include "masterpty.h"
#include "ptymultiplexer.h"

namespace Kernel
{
    static const unsigned s_max_pty_pairs = 8;

    static Mods::Singleton<PTYMultiplexer> s_the;

    /**
     * @return PTYMultiplexer& 
     */
    PTYMultiplexer& PTYMultiplexer::the()
    {
        return *s_the;
    }

    /// @brief Construct a new PTYMultiplexer::PTYMultiplexer object
    PTYMultiplexer::PTYMultiplexer()
        : CharacterDevice(5, 2)
    {
        m_freelist.ensure_capacity(s_max_pty_pairs);

        for (int i = s_max_pty_pairs; i > 0; --i)
            m_freelist.unchecked_append(i - 1);
    }

    /// @brief Destroy the PTYMultiplexer::PTYMultiplexer object
    PTYMultiplexer::~PTYMultiplexer()
    {}

} // namespace Kernel