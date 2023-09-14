/**
 * @file sharedbuffer.cpp
 * @author Krisna Pranav
 * @brief shared buffer
 * @version 6.0
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/singleton.h>
#include <kernel/process.h>
#include <kernel/sharedbuffer.h>

namespace Kernel
{
    /**
     * @return Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& 
     */
    Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& shared_buffer()
    {
        return *s_map;
    }
    
    /**
     * @param what 
     */
    void SharedBuffer::sanity_check(const char* what)
    {
        LOCKER(shared_buffer().lock(), Lock::Mode::Shared);

        unsigned found_refs = 0;

        for (const auto& ref : m_refs)
            founds_refs += ref.count;
        
        if (found_refs != m_total_refs) {
            dbg() << what << ""

            for (const auto& ref : m_refs) {
                dbg() << ref.pid.value();
            }

            ASSERT_NOT_REACHED();
        }
    }
}