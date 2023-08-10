/**
 * @file file.h
 * @author Krisna Pranav
 * @brief file
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/weakable.h>
#include <kernel/forward.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>
#include <kernel/virtual_address.h>
#include <kernel/userorkernelbuffer.h>

namespace Kernel 
{

    class File;

    class FileBlockCondition : public Thread::BlockCondition 
    {
    public:
        /**
         * @param file 
         */
        FileBlockCondition(File& file)
            : m_file(file)
        {
        }

        /**
         * @param b 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool should_add_blocker(Thread::Blocker& b, void* data) override
        {
            ASSERT(b.blocker_type() == Thread::Blocker::Type::File);
            auto& blocker = static_cast<Thread::FileBlocker&>(b);
            return !blocker.unblock(true, data);
        }

        /// @brief: unblock
        void unblock()
        {
            ScopedSpinLock lock(m_lock);
            do_unblock([&](auto& b, void* data) {
                ASSERT(b.blocker_type() == Thread::Blocker::Type::File);
                auto& blocker = static_cast<Thread::FileBlocker&>(b);
                return blocker.unblock(false, data);
            });
        }

    private:
        File& m_file;
    }; // class FileBlockCondition

    class File
        : public RefCounted<File>
        , public Weakable<File> {
    public:
        virtual ~File();

        /**
         * @param options 
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        virtual KResultOr<NonnullRefPtr<FileDescription>> open(int options);

        /**
         * @return KResult 
         */
        virtual KResult close();

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const = 0;
        virtual bool can_write(const FileDescription&, size_t) const = 0;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) = 0;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) = 0;

        /**
         * @param request 
         * @param arg 
         * @return int 
         */
        virtual int ioctl(FileDescription&, unsigned request, FlatPtr arg);

        /**
         * @param preferred_vaddr 
         * @param offset 
         * @param size 
         * @param prot 
         * @param shared 
         * @return KResultOr<Region*> 
         */
        virtual KResultOr<Region*> mmap(Process&, FileDescription&, VirtualAddress preferred_vaddr, size_t offset, size_t size, int prot, bool shared);

        /**
         * @return KResult 
         */
        virtual KResult stat(::stat&) const 
        { 
            return KResult(-EBADF); 
        }

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const = 0;

        /**
         * @return KResult 
         */
        virtual KResult truncate(u64) 
        { 
            return KResult(-EINVAL); 
        }

        /**
         * @return KResult 
         */
        virtual KResult chown(FileDescription&, uid_t, gid_t) 
        { 
            return KResult(-EBADF); 
        }

        /**
         * @return KResult 
         */
        virtual KResult chmod(FileDescription&, mode_t) 
        { 
            return KResult(-EBADF); 
        }

        /**
         * @return const char* 
         */
        virtual const char* class_name() const = 0;

        virtual bool is_seekable() const 
        { 
            return false; 
        }

        virtual bool is_inode() const 
        { 
            return false; 
        }

        virtual bool is_fifo() const 
        { 
            return false; 
        }

        virtual bool is_device() const 
        { 
            return false; 
        }

        virtual bool is_tty() const 
        { 
            return false; 
        }

        virtual bool is_master_pty() const 
        { 
            return false; 
        }

        virtual bool is_block_device() const 
        { 
            return false; 
        }

        virtual bool is_character_device() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_socket() const 
        { 
            return false; 
        }

        /**
         * @return FileBlockCondition& 
         */
        virtual FileBlockCondition& block_condition() 
        { 
            return m_block_condition; 
        }

    protected:
        File();

        /// @brief: evaluate_block_conditions
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

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void do_evaluate_block_conditions()
        {
            ASSERT(!Processor::current().in_irq());
            block_condition().unblock();
        }

        FileBlockCondition m_block_condition;
    }; // class File

} // namespace Kernel
