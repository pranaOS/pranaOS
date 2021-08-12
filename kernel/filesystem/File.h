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

namespace Kernel {

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

    virtual KResultOr<NonnullRefPtr<FileDescription>> open(int options);
    virtual KResult close();

    virtual bool can_read(const FileDescription&, size_t) const = 0;
    virtual bool can_write(const FileDescription&, size_t) const = 0;

    virtual KResult attach(FileDescription&);
    virtual void detach(FileDescription&);
    virtual void did_seek(FileDescription&, off_t) { }
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) = 0;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) = 0;
    virtual KResult ioctl(FileDescription&, unsigned request, Userspace<void*> arg);
    virtual KResultOr<Memory::Region*> mmap(Process&, FileDescription&, Memory::VirtualRange const&, u64 offset, int prot, bool shared);
    virtual KResult stat(::stat&) const { return EBADF; }

    virtual String absolute_path(const FileDescription&) const = 0;

    virtual KResult truncate(u64) { return EINVAL; }
    virtual KResult chown(FileDescription&, uid_t, gid_t) { return EBADF; }
    virtual KResult chmod(FileDescription&, mode_t) { return EBADF; }

    virtual StringView class_name() const = 0;

    virtual bool is_seekable() const { return false; }

    virtual bool is_inode() const { return false; }
    virtual bool is_fifo() const { return false; }
    virtual bool is_device() const { return false; }
    virtual bool is_tty() const { return false; }
    virtual bool is_master_pty() const { return false; }
    virtual bool is_block_device() const { return false; }
    virtual bool is_character_device() const { return false; }
    virtual bool is_socket() const { return false; }
    virtual bool is_inode_watcher() const { return false; }

    virtual FileBlockCondition& block_condition() { return m_block_condition; }

    size_t attach_count() const { return m_attach_count; }

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

private:
    ALWAYS_INLINE void do_evaluate_block_conditions()
    {
        VERIFY(!Processor::current().in_irq());
        block_condition().unblock();
    }

    FileBlockCondition m_block_condition;
    size_t m_attach_count { 0 };
};

}
