/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/IntrusiveList.h>
#include <base/RefCounted.h>
#include <base/Weakable.h>
#include <kernel/locking/SpinLockProtectedValue.h>
#include <kernel/UnixTypes.h>

namespace Kernel {

class ProcessGroup
    : public RefCounted<ProcessGroup>
    , public Weakable<ProcessGroup> {

    BASE_MAKE_NONMOVABLE(ProcessGroup);
    BASE_MAKE_NONCOPYABLE(ProcessGroup);

public:
    ~ProcessGroup();

    static RefPtr<ProcessGroup> create(ProcessGroupID);
    static RefPtr<ProcessGroup> find_or_create(ProcessGroupID);
    static RefPtr<ProcessGroup> from_pgid(ProcessGroupID);

    const ProcessGroupID& pgid() const { return m_pgid; }

private:
    ProcessGroup(ProcessGroupID pgid)
        : m_pgid(pgid)
    {
    }

    IntrusiveListNode<ProcessGroup> m_list_node;
    ProcessGroupID m_pgid;

public:
    using List = IntrusiveList<ProcessGroup, RawPtr<ProcessGroup>, &ProcessGroup::m_list_node>;
};

SpinLockProtectedValue<ProcessGroup::List>& process_groups();

}
