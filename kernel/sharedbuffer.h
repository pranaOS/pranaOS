/**
 * @file sharedbuffer.h
 * @author Krisna Pranav
 * @brief shared buffer
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/ownptr.h>
#include <mods/weakptr.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/purgeablevmobject.h>

namespace Kernel
{
    class SharedBuffer
    {
    private:
        struct Reference
        {
            Reference(ProcessID pid, unsigned count = 0)
                : pid(pid)
                , count(count)
            {}
        };

    private:
        int m_sbhuf_id { -1 };
        bool m_writable { true };
        bool m_global { false };
        NonnullRefPtr<PurgeableVMObject> m_vmobject;
        Vector<Reference, 2> m_refs;

        unsigned m_total_refs { 0 };
    }; // class SharedBuffer

    Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& shared_buffers();
}