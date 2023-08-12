/**
 * @file process.h
 * @author Krisna Pranav
 * @brief process
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libc/signal_codes.h>
#include <libelf/auxvector.h>
#include <mods/hashmap.h>
#include <mods/check.h>
#include <mods/inlinelinkedlist.h>
#include <mods/nonnullownptrvector.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/string.h>
#include <mods/usrspace.h>
#include <mods/weakable.h>
#include <mods/weakptr.h>
#include <kernel/forward.h>
#include <kernel/lock.h>
#include <kernel/processgroup.h>
#include <kernel/stdlib.h>
#include <kernel/thread.h>
#include <kernel/threadtracer.h>
#include <kernel/unixtypes.h>
#include <kernel/api/syscall.h>
#include <kernel/filesystem/inodemetadata.h>
#include <kernel/vm/rangeallocator.h>

namespace ELF
{
    class Loader;
} // namespace ELF

namespace Kernel
{

    timeval kgettimeofday();
    void kgettimeofday(timeval&);

    enum class Pledge : u32 
    {
    #define __ENUMERATE_PLEDGE_PROMISE(x) x,
        ENUMERATE_PLEDGE_PROMISE
    #undef __ENUMERATE_PLEDGE_PROMISE
    };

    enum class VeilState 
    {
        None,
        Dropped,
        Locked
    };

    struct UnveiledPath
    {
        enum Access
        {
            Read = 1,
            Write = 2,
            Execute = 4,
            CreateOrRemove = 8,
            Browse = 16,
        }; 

        String path;

        unsigned permissions { 0 };
    }; // struct 

    class Process : public RefCounted<Process>
        , public InlineLinkedListNode<Process>
        , public Weakable<Process> {
        
        MOD_MAKE_NONCOPYABLE(Process);
        MOD_MAKE_NONMOVABLE(Process);

        friend class InlineLinkedListNode<Process>;
        friend class Thread;
        friend class CoreDump;

    private:
        friend class MemoryManager;
        friend class Scheduler;
        friend class Region;

        /**
         * @param first_thread 
         * @param name 
         * @param ppid 
         * @param is_kernel_process 
         * @param cwd 
         * @param executable 
         * @param fork_parent 
         */
        Process(RefPtr<Thread>& first_thread, const String& name, uid_t, gid_t, ProcessID ppid, bool is_kernel_process, RefPtr<Custody> cwd = nullptr, RefPtr<Custody> executable = nullptr, TTY* = nullptr, Process* fork_parent = nullptr);

        static ProcessID allocate_pid();

        /**
         * @param alignment 
         * @return Range 
         */
        Range allocate_range(VirtualAddress, size_t, size_t alignment = PAGE_SIZE);

        /**
         * @return Region& 
         */
        Region& add_region(NonnullOwnPtr<Region>);

        void kill_threads_except_self();
        void kill_all_threads();

        /**
         * @param main_program_description 
         * @param arguments 
         * @param environment 
         * @param interpreter_description 
         * @param new_main_thread 
         * @param prev_flags 
         * @return int 
         */
        int do_exec(NonnullRefPtr<FileDescription> main_program_description, Vector<String> arguments, Vector<String> environment, RefPtr<FileDescription> interpreter_description, Thread*& new_main_thread, u32& prev_flags);

        /**
         * @return ssize_t 
         */
        ssize_t do_write(FileDescription&, const UserOrKernelBuffer&, size_t);      

        /**
         * @param path 
         * @param nread 
         * @param file_size 
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        KResultOr<NonnullRefPtr<FileDescription>> find_elf_interpreter_for_executable(const String& path, char (&first_page)[PAGE_SIZE], int nread, size_t file_size);

        Vector<AuxiliaryValue> generate_auxiliary_vector() const;

        /**
         * @param first_canditate_fd 
         * @return int 
         */
        int alloc_fd(int first_canditate_fd = 0);   

        void disown_all_shared_buffers();

    }; // class Process

} // namespace Kernel