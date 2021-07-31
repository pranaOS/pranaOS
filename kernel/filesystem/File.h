/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullRefPtr.h>
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/Types.h>
#include <base/Weakable.h>
#include <kernel/Forward.h>
#include <kernel/KResult.h>
#include <kernel/UnixTypes.h>
#include <kernel/UserOrKernelBuffer.h>
#include <kernel/VirtualAddress.h>

namespace Kernel
{

class File;

class FileBlockCondition : public Thread::BlockCondition {
public:
    FileBlockCondition() { }

    virtual bool should_add_blocker(Thread::Blocker& b, void* data) override
    {
        VERIFY(b.blocker_type() == Thread::Blocker::Type::File);
        auto& blocker = static_cast<Thread::FileBlocker&>(b);
        return !blocker.unblock(true, data);
    }

    void unblock()
    {
        ScopedSpinLock lock(m_lock);
        do_unblock([&](auto& b, void* data, bool&) {
            VERIFY(b.blocker_type() == Thread::Blocker::Type::File);
            auto& blocker = static_cast<Thread::FileBlocker&>(b);
            return blocker.unblock(false, data);
        });
    }
};

class File
    : public RefCounted<File>
    , public Weakable<File> {
public:
    virtual ~File();

    virtual KResultOr<NonnullRefPtr<FileDescriptor>> open(int options);
    virtual KResult close();

protected:
    File();

    void evaluate_block_conditions()
    {
        if (Processor::current().in_irq()) {
            Processor::deferred_call_queue([self = make_weak_ptr()]() {
                if (auto file = self.strong_ref())
                    file->do_evaluate_block_conditions();
            });
        } else {
            do_evaluate_block_conditions();
        }
    }

    }
   
};
