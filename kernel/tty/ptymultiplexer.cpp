/**
 * @file ptymultiplexer.cpp
 * @author Krisna Pranav
 * @brief pty multi plexer
 * @version 6.0
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
    /// @brief: s_max_pty_pairs
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
    { }
    
    /**
     * @param options 
     * @return KResultOr<NonnullRefPtr<FileDescription>> 
     */
    KResultOr<NonnullRefPtr<FileDescription>> PTYMultiplexer::open(int options)
    {
        LOCKER(m_lock);

        if (m_freelist.is_empty())
            return KResult(-EBUSY);

        auto master_index = m_freelist.take_last();
        auto master = adopt(*new MasterPTY(master_index));

    #ifdef PTMX_DEBUG
        dbg() << "PTYMultiplexer::open: Vending master " << master->index();
    #endif

        auto description = FileDescription::create(move(master));

        description->set_rw_mode(options);
        description->set_file_flags(options);

        return description;
    }

    /**
     * @param index 
     */
    void PTYMultiplexer::notify_master_destroyed(Badge<MasterPTY>, unsigned index)
    {
        LOCKER(m_lock);
        
        m_freelist.append(index);

    #ifdef PTMX_DEBUG
        dbg() << "PTYMultiplexer: " << index << " added to freelist";
    #endif
    }

} // namespace Mods
