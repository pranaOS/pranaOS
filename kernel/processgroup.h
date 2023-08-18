/**
 * @file processgroup.h
 * @author Krisna Pranav
 * @brief Processing Group
 * @version 6.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/inlinelinkedlist.h>
#include <mods/refcounted.h>
#include <mods/weakable.h>
#include <kernel/lock.h>
#include <kernel/spinlock.h>
#include <kernel/unixtypes.h>

namespace Kernel 
{

    class ProcessGroup : public RefCounted<ProcessGroup>
        , public Weakable<ProcessGroup>
        , public InlineLinkedListNode<ProcessGroup> {

        MOD_MAKE_NONMOVABLE(ProcessGroup);
        MOD_MAKE_NONCOPYABLE(ProcessGroup);

        friend InlineLinkedListNode<ProcessGroup>;

    public:
        ~ProcessGroup();

        /**
         * @return NonnullRefPtr<ProcessGroup> 
         */
        static NonnullRefPtr<ProcessGroup> create(ProcessGroupID);

        /**
         * @return NonnullRefPtr<ProcessGroup> 
         */
        static NonnullRefPtr<ProcessGroup> find_or_create(ProcessGroupID);

        /**
         * @return RefPtr<ProcessGroup> 
         */
        static RefPtr<ProcessGroup> from_pgid(ProcessGroupID);

        /**
         * @return const ProcessGroupID& 
         */
        const ProcessGroupID& pgid() const 
        { 
            return m_pgid; 
        }

    private:
        /**
         * @param pgid 
         */
        ProcessGroup(ProcessGroupID pgid)
            : m_pgid(pgid)
        {
        }

        /// @brief prev + next
        ProcessGroup* m_prev { nullptr };
        ProcessGroup* m_next { nullptr };

        ProcessGroupID m_pgid;
    }; // class ProcessGroup

    extern InlineLinkedList<ProcessGroup>* g_process_groups;
    extern RecursiveSpinLock g_process_groups_lock;

}
