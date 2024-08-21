/**
 * @file thread.h
 * @author Krisna Pranav
 * @brief thread
 * @version 6.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/arch/i386/cpu.h>
#include <kernel/kresult.h>
#include <kernel/scheduler.h>
#include <kernel/threadtracer.h>
#include <kernel/timerqueue.h>
#include <kernel/unixtypes.h>
#include <mods/function.h>
#include <mods/intrusivelist.h>
#include <mods/optional.h>
#include <mods/ownptr.h>
#include <mods/string.h>
#include <mods/time.h>
#include <mods/vector.h>
#include <mods/weakptr.h>
#include <mods/weakable.h>
#include <libc/fd_set.h>
#include <libelf/auxvector.h>

namespace Kernel 
{

    /// @brief: dispatch signal result codes.
    enum class DispatchSignalResult 
    {
        Deferred = 0,
        Yield,
        Terminate,
        Continue
    };

    /// @brief: signal action data.
    struct SignalActionData 
    {
        VirtualAddress handler_or_sigaction;
        u32 mask { 0 };
        int flags { 0 };
    }; // struct

    struct ThreadSpecificData 
    {
        ThreadSpecificData* self;
    };

    #define THREAD_PRIORITY_MIN 1
    #define THREAD_PRIORITY_LOW 10
    #define THREAD_PRIORITY_NORMAL 30
    #define THREAD_PRIORITY_HIGH 50
    #define THREAD_PRIORITY_MAX 99
    #define THREAD_AFFINITY_DEFAULT 0xffffffff

    class Thread
        : public RefCounted<Thread>
        , public Weakable<Thread> {

        MOD_MAKE_NONCOPYABLE(Thread);
        MOD_MAKE_NONMOVABLE(Thread);

        friend class Process;
        friend class Scheduler;

    public:
        /**
         * @return Thread* 
         */
        inline static Thread* current()
        {
            return Processor::current().current_thread();
        }

        explicit Thread(NonnullRefPtr<Process>);
        ~Thread();

        /**
         * @return RefPtr<Thread> 
         */
        static RefPtr<Thread> from_tid(ThreadID);
        static void finalize_dying_threads();
        
        /**
         * @return ThreadID 
         */
        ThreadID tid() const 
        { 
            return m_tid; 
        }

        /**
         * @return ProcessID 
         */
        ProcessID pid() const;

        /**
         * @param p 
         */
        void set_priority(u32 p) 
        { 
            m_priority = p; 
        }

        /**
         * @return u32 
         */
        u32 priority() const 
        { 
            return m_priority; 
        }

        /**
         * @param boost 
         */
        void set_priority_boost(u32 boost) 
        { 
            m_priority_boost = boost; 
        }

        /**
         * @return u32 
         */
        u32 priority_boost() const 
        { 
            return m_priority_boost; 
        }

        /**
         * @return u32 
         */
        u32 effective_priority() const;

        void detach()
        {
            ScopedSpinLock lock(m_lock);
            m_is_joinable = false;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_joinable() const
        {
            ScopedSpinLock lock(m_lock);
            return m_is_joinable;
        }

        /**
         * @return Process& 
         */
        Process& process() 
        { 
            return m_process; 
        }

        /**
         * @return const Process& 
         */
        const Process& process() const 
        { 
            return m_process; 
        }

        /**
         * @return String 
         */
        String backtrace();

        /**
         * @param ebp 
         * @param eip 
         * @return Vector<FlatPtr> 
         */
        Vector<FlatPtr> raw_backtrace(FlatPtr ebp, FlatPtr eip) const;

        /**
         * @return String 
         */
        String name() const
        {
            ScopedSpinLock lock(m_lock);
            return m_name;
        }

        /**
         * @param s 
         */
        void set_name(const StringView& s)
        {
            ScopedSpinLock lock(m_lock);
            m_name = s;
        }

        /**
         * @param name 
         */
        void set_name(String&& name)
        {
            ScopedSpinLock lock(m_lock);
            m_name = move(name);
        }

        void finalize();

        /// @breif: State
        enum State : u8 
        {
            Invalid = 0,
            Runnable,
            Running,
            Dying,
            Dead,
            Stopped,
            Blocked
        }; // enum

        class BlockResult 
        {
        public:
            enum Type 
            {
                WokeNormally,
                NotBlocked,
                InterruptedBySignal,
                InterruptedByDeath,
                InterruptedByTimeout,
            }; // enum Type

            /// @brief: construct
            BlockResult() = delete;

            /**
             *
             * @param type 
             */
            BlockResult(Type type)
                : m_type(type)
            {
            }

            bool operator==(Type type) const
            {
                return m_type == type;
            }
            bool operator!=(Type type) const
            {
                return m_type != type;
            }

            bool was_interrupted() const
            {
                switch (m_type) {
                case InterruptedBySignal:
                case InterruptedByDeath:
                    return true;
                default:
                    return false;
                }
            }

            bool timed_out() const
            {
                return m_type == InterruptedByTimeout;
            }

        private:
            Type m_type;
        };

        class BlockTimeout {
        public:
            BlockTimeout()
                : m_infinite(true)
            {
            }
            BlockTimeout(std::nullptr_t)
                : m_infinite(true)
            {
            }
            explicit BlockTimeout(bool is_absolute, const timeval* time, const timespec* start_time = nullptr, clockid_t clock_id = CLOCK_MONOTONIC)
                : m_clock_id(clock_id)
                , m_infinite(!time)
            {
                if (!m_infinite) {
                    if (time->tv_sec > 0 || time->tv_usec > 0) {
                        timeval_to_timespec(*time, m_time);
                        m_should_block = true;
                    }
                    m_start_time = start_time ? *start_time : TimeManagement::the().current_time(clock_id).value();
                    if (!is_absolute)
                        timespec_add(m_time, m_start_time, m_time);
                }
            }
            explicit BlockTimeout(bool is_absolute, const timespec* time, const timespec* start_time = nullptr, clockid_t clock_id = CLOCK_MONOTONIC)
                : m_clock_id(clock_id)
                , m_infinite(!time)
            {
                if (!m_infinite) {
                    if (time->tv_sec > 0 || time->tv_nsec > 0) {
                        m_time = *time;
                        m_should_block = true;
                    }
                    m_start_time = start_time ? *start_time : TimeManagement::the().current_time(clock_id).value();
                    if (!is_absolute)
                        timespec_add(m_time, m_start_time, m_time);
                }
            }

            const timespec& absolute_time() const { return m_time; }
            const timespec* start_time() const { return !m_infinite ? &m_start_time : nullptr; }
            clockid_t clock_id() const { return m_clock_id; }
            bool is_infinite() const { return m_infinite; }
            bool should_block() const { return m_infinite || m_should_block; };

        private:
            timespec m_time { 0, 0 };
            timespec m_start_time { 0, 0 };
            clockid_t m_clock_id { CLOCK_MONOTONIC };
            bool m_infinite { false };
            bool m_should_block { false };
        };

        class BlockCondition;

        class Blocker 
        {
        public:
            enum class Type 
            {
                Unknown = 0,
                File,
                Plan9FS,
                Join,
                Queue,
                Routing,
                Sleep,
                Wait
            };

            virtual ~Blocker();

            /**
             * @return const char* 
             */
            virtual const char* state_string() const = 0;

            /**
             * @return true 
             * @return false 
             */
            virtual bool should_block() 
            { 
                return true; 
            }

            /**
             * @return Type 
             */
            virtual Type blocker_type() const = 0;

            /**
             * @param timeout 
             * @return const BlockTimeout& 
             */
            virtual const BlockTimeout& override_timeout(const BlockTimeout& timeout) 
            { 
                return timeout; 
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool can_be_interrupted() const 
            { 
                return true; 
            }

            virtual void not_blocking(bool) = 0;

            /**
             * @param did_timeout 
             */
            virtual void was_unblocked(bool did_timeout)
            {
                if (did_timeout) {
                    ScopedSpinLock lock(m_lock);
                    m_did_timeout = true;
                }
            }

            void set_interrupted_by_death()
            {
                ScopedSpinLock lock(m_lock);
                do_set_interrupted_by_death();
            }

            /**
             * @param signal 
             */
            void set_interrupted_by_signal(u8 signal)
            {
                ScopedSpinLock lock(m_lock);
                do_set_interrupted_by_signal(signal);
            }

            /**
             * @return u8 
             */
            u8 was_interrupted_by_signal() const
            {
                ScopedSpinLock lock(m_lock);
                return do_get_interrupted_by_signal();
            }

            /**
             * @return Thread::BlockResult 
             */
            virtual Thread::BlockResult block_result()
            {
                ScopedSpinLock lock(m_lock);
                if (m_was_interrupted_by_death)
                    return Thread::BlockResult::InterruptedByDeath;
                if (m_was_interrupted_by_signal != 0)
                    return Thread::BlockResult::InterruptedBySignal;
                if (m_did_timeout)
                    return Thread::BlockResult::InterruptedByTimeout;
                return Thread::BlockResult::WokeNormally;
            }

            void begin_blocking(Badge<Thread>);

            /**
             * @return BlockResult 
             */
            BlockResult end_blocking(Badge<Thread>, bool);

        protected:

            void do_set_interrupted_by_death()
            {
                m_was_interrupted_by_death = true;
            }

            /**
             * @param signal 
             */
            void do_set_interrupted_by_signal(u8 signal)
            {
                ASSERT(signal != 0);
                m_was_interrupted_by_signal = signal;
            }

            /// @brief: do_clear_interrupted_by_signal = 0;
            void do_clear_interrupted_by_signal()
            {
                m_was_interrupted_by_signal = 0;
            }

            /**
             * @return u8 
             */
            u8 do_get_interrupted_by_signal() const
            {
                return m_was_interrupted_by_signal;
            }

            /**
             * @return true 
             * @return false 
             */
            bool was_interrupted() const
            {
                return m_was_interrupted_by_death || m_was_interrupted_by_signal != 0;
            }

            /// @brief: unblock from blocker
            void unblock_from_blocker()
            {
                RefPtr<Thread> thread;

                {
                    ScopedSpinLock lock(m_lock);
                    if (m_is_blocking) {
                        m_is_blocking = false;
                        ASSERT(m_blocked_thread);
                        thread = m_blocked_thread;
                    }
                }

                if (thread)
                    thread->unblock_from_blocker(*this);
            }

            /**
             * @return true 
             * @return false 
             */
            bool set_block_condition(BlockCondition&, void* = nullptr);

            mutable RecursiveSpinLock m_lock;

        private:
            BlockCondition* m_block_condition { nullptr };
            void* m_block_data { nullptr };
            Thread* m_blocked_thread { nullptr };
            u8 m_was_interrupted_by_signal { 0 };
            bool m_is_blocking { false };
            bool m_was_interrupted_by_death { false };
            bool m_did_timeout { false };
        };

        class BlockCondition 
        {
            MOD_MAKE_NONCOPYABLE(BlockCondition);
            MOD_MAKE_NONMOVABLE(BlockCondition);

        public:
            BlockCondition() = default;

            /**
             * @brief Destroy the Block Condition object
             * 
             */
            virtual ~BlockCondition()
            {
                ScopedSpinLock lock(m_lock);
                ASSERT(m_blockers.is_empty());
            }

            /**
             * @param blocker 
             * @param data 
             * @return true 
             * @return false 
             */
            bool add_blocker(Blocker& blocker, void* data)
            {
                ScopedSpinLock lock(m_lock);

                if (!should_add_blocker(blocker, data))
                    return false;

                m_blockers.append({ &blocker, data });
                return true;
            }

            /**
             * @param blocker 
             * @param data 
             */
            void remove_blocker(Blocker& blocker, void* data)
            {
                ScopedSpinLock lock(m_lock);

                m_blockers.remove_first_matching([&](auto& info) {
                    return info.blocker == &blocker && info.data == data;
                });
            }

        protected:
            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             */
            template<typename UnblockOne>
            void unblock(UnblockOne unblock_one)
            {
                ScopedSpinLock lock(m_lock);
                do_unblock(unblock_one);
            }

            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             */
            template<typename UnblockOne>
            void do_unblock(UnblockOne unblock_one)
            {
                ASSERT(m_lock.is_locked());

                for (size_t i = 0; i < m_blockers.size();) {
                    auto& info = m_blockers[i];
                    if (unblock_one(*info.blocker, info.data)) {
                        m_blockers.remove(i);
                        continue;
                    }

                    i++;
                }
            }

            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             * @return true 
             * @return false 
             */
            template<typename UnblockOne>
            bool unblock_some(UnblockOne unblock_one)
            {
                ScopedSpinLock lock(m_lock);
                return do_unblock_some(unblock_one);
            }

            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             * @return true 
             * @return false 
             */
            template<typename UnblockOne>
            bool do_unblock_some(UnblockOne unblock_one)
            {
                ASSERT(m_lock.is_locked());
                bool stop_iterating = false;

                for (size_t i = 0; i < m_blockers.size() && !stop_iterating;) {
                    auto& info = m_blockers[i];
                    if (unblock_one(*info.blocker, info.data, stop_iterating)) {
                        m_blockers.remove(i);
                        continue;
                    }

                    i++;
                }
                return !stop_iterating;
            }

            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             * @return true 
             * @return false 
             */
            template<typename UnblockOne>
            bool unblock_all(UnblockOne unblock_one)
            {
                ScopedSpinLock lock(m_lock);
                return do_unblock_all(unblock_one);
            }

            /**
             * @tparam UnblockOne 
             * @param unblock_one 
             * @return true 
             * @return false 
             */
            template<typename UnblockOne>
            bool do_unblock_all(UnblockOne unblock_one)
            {
                ASSERT(m_lock.is_locked());
                bool unblocked_any = false;

                for (auto& info : m_blockers) {
                    bool did_unblock = unblock_one(*info.blocker, info.data);
                    unblocked_any |= did_unblock;
                    ASSERT(did_unblock);
                }

                m_blockers.clear();
                return unblocked_any;
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool should_add_blocker(Blocker&, void*) 
            { 
                return true; 
            }

            SpinLock<u8> m_lock;

        private:
            struct BlockerInfo 
            {
                Blocker* blocker;
                void* data;
            };
            Vector<BlockerInfo, 4> m_blockers;
        };

        friend class JoinBlocker;
        class JoinBlocker final : public Blocker 
        {
        public:
            /**
             * @param joinee 
             * @param try_join_result 
             * @param joinee_exit_value 
             */
            explicit JoinBlocker(Thread& joinee, KResult& try_join_result, void*& joinee_exit_value);

            /**
             * @return Type 
             */
            virtual Type blocker_type() const override 
            { 
                return Type::Join; 
            }

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return "Joining"; 
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool can_be_interrupted() const override 
            { 
                return false; 
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool should_block() override 
            { 
                return !m_join_error && m_should_block; 
            }

            virtual void not_blocking(bool) override;

            bool unblock(void*, bool);

        private:
            NonnullRefPtr<Thread> m_joinee;
            void*& m_joinee_exit_value;
            bool m_join_error { false };
            bool m_did_unblock { false };
            bool m_should_block { true };
        }; // class JoinBlocker

        class QueueBlocker : public Blocker 
        {
        public:
            /**
             * @param block_reason 
             */
            explicit QueueBlocker(WaitQueue&, const char* block_reason = nullptr);
            virtual ~QueueBlocker();

            /**
             * @return Type 
             */
            virtual Type blocker_type() const override 
            { 
                return Type::Queue; 
            }

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return m_block_reason ? m_block_reason : "Queue"; 
            }

            virtual void not_blocking(bool) override { }

            virtual bool should_block() override
            {
                return m_should_block;
            }

            bool unblock();

        protected:
            const char* const m_block_reason;
            bool m_should_block { true };
            bool m_did_unblock { false };
        }; // class QueueBlocker

        class FileBlocker : public Blocker 
        {
        public:
            enum class BlockFlags : u32 
            {
                None = 0,

                Read = 1 << 0,
                Write = 1 << 1,
                ReadPriority = 1 << 2,

                Accept = 1 << 3,
                Connect = 1 << 4,
                SocketFlags = Accept | Connect,

                WriteNotOpen = 1 << 5,
                WriteError = 1 << 6,
                WriteHangUp = 1 << 7,
                ReadHangUp = 1 << 8,
                Exception = WriteNotOpen | WriteError | WriteHangUp | ReadHangUp,
            }; // enum 

            /**
             * @return Type 
             */
            virtual Type blocker_type() const override 
            { 
                return Type::File; 
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool should_block() override
            {
                return m_should_block;
            }

            /**
             * @return true 
             * @return false 
             */
            virtual bool unblock(bool, void*) = 0;

        protected:
            bool m_should_block { true };
        }; // class FileBlocker

        class FileDescriptionBlocker : public FileBlocker 
        {
        public: 
            /**
             * @return const FileDescription& 
             */
            const FileDescription& blocked_description() const;

            /**
             * @return true 
             * @return false 
             */
            virtual bool unblock(bool, void*) override;
            virtual void not_blocking(bool) override;

        protected:
            explicit FileDescriptionBlocker(FileDescription&, BlockFlags, BlockFlags&);

        private:
            NonnullRefPtr<FileDescription> m_blocked_description;
            const BlockFlags m_flags;
            BlockFlags& m_unblocked_flags;
            bool m_did_unblock { false };
            bool m_should_block { true };
        }; // class FileDescriptionBlocker

        class AcceptBlocker final : public FileDescriptionBlocker 
        {
        public:
            explicit AcceptBlocker(FileDescription&, BlockFlags&);

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return "Accepting"; 
            }
        }; // class AcceptBlocker

        class ConnectBlocker final : public FileDescriptionBlocker {
        public:
            explicit ConnectBlocker(FileDescription&, BlockFlags&);
            virtual const char* state_string() const override { return "Connecting"; }
        };

        class WriteBlocker final : public FileDescriptionBlocker 
        {
        public:
            /// @brief: Construct a new Write Blocker Object.
            explicit WriteBlocker(FileDescription&, BlockFlags&);

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return "Writing"; 
            }

            /**
             * @return const BlockTimeout& 
             */
            virtual const BlockTimeout& override_timeout(const BlockTimeout&) override;

        private:
            BlockTimeout m_timeout;
        }; // class WriteBlocker

        class ReadBlocker final : public FileDescriptionBlocker 
        {
        public:
            /// @breif: Construct a new Read Blocker Object
            explicit ReadBlocker(FileDescription&, BlockFlags&);

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return "Reading"; 
            }

            /**
             * @return const BlockTimeout& 
             */
            virtual const BlockTimeout& override_timeout(const BlockTimeout&) override;

        private:
            BlockTimeout m_timeout;
        }; // class ReadBlocker

        class SleepBlocker final : public Blocker 
        {
        public:
            /// @brief: Construct a new Sleep Blocker object
            explicit SleepBlocker(const BlockTimeout&, timespec* = nullptr);

            /**
             * @return const char* 
             */
            virtual const char* state_string() const override 
            { 
                return "Sleeping"; 
            }

            /**
             * @return Type 
             */
            virtual Type blocker_type() const override 
            { 
                return Type::Sleep; 
            }

            /**
             * @return const BlockTimeout& 
             */
            virtual const BlockTimeout& override_timeout(const BlockTimeout&) override;

            virtual void not_blocking(bool) override;
            virtual void was_unblocked(bool) override;

            /**
             * @return Thread::BlockResult 
             */
            virtual Thread::BlockResult block_result() override;

        private:
            void calculate_remaining();

            BlockTimeout m_deadline;
            timespec* m_remaining;
        };

        class SelectBlocker final : public FileBlocker 
        {
        public:
            struct FDInfo 
            {
                NonnullRefPtr<FileDescription> description;
                BlockFlags block_flags;
                BlockFlags unblocked_flags { BlockFlags::None };
            };

            typedef Vector<FDInfo, FD_SETSIZE> FDVector;

            /**
             * @param fds 
             */
            SelectBlocker(FDVector& fds);

            virtual ~SelectBlocker();

            virtual bool unblock(bool, void*) override;
            virtual void not_blocking(bool) override;
            virtual void was_unblocked(bool) override;

            virtual const char* state_string() const override 
            { 
                return "Selecting"; 
            }

        private:
            size_t collect_unblocked_flags();

            FDVector& m_fds;
            bool m_did_unblock { false };
        }; // class SelectBlocker

        class WaitBlocker final : public Blocker {
        public:

            enum class UnblockFlags 
            {
                Terminated,
                Stopped,
                Continued,
                Disowned
            }; // UnblockFlags

            /**
             * @param wait_options 
             * @param id_type 
             * @param id 
             * @param result 
             */
            WaitBlocker(int wait_options, idtype_t id_type, pid_t id, KResultOr<siginfo_t>& result);

            virtual const char* state_string() const override 
            { 
                return "Waiting"; 
            }

            /**
             * @return Type 
             */
            virtual Type blocker_type() const override 
            { 
                return Type::Wait; 
            }

            virtual bool should_block() override 
            { 
                return m_should_block; 
            }

            virtual void not_blocking(bool) override;
            virtual void was_unblocked(bool) override;

            /**
             * @param process 
             * @param flags 
             * @param signal 
             * @param from_add_blocker 
             * @return true 
             * @return false 
             */
            bool unblock(Process& process, UnblockFlags flags, u8 signal, bool from_add_blocker);

            /**
             * @return true 
             * @return false 
             */
            bool is_wait() const 
            { 
                return !(m_wait_options & WNOWAIT); 
            }

        private:
            void do_was_disowned();
            void do_set_result(const siginfo_t&);

            const int m_wait_options;
            const idtype_t m_id_type;
            const pid_t m_waitee_id;
            KResultOr<siginfo_t>& m_result;
            RefPtr<Process> m_waitee;
            RefPtr<ProcessGroup> m_waitee_group;
            bool m_did_unblock { false };
            bool m_error { false };
            bool m_got_sigchild { false };
            bool m_should_block;
        };

        class WaitBlockCondition final : public BlockCondition 
        {
            friend class WaitBlocker;

        public:
            /**
             * @param process 
             */
            WaitBlockCondition(Process& process)
                : m_process(process)
            {
            }

            void disowned_by_waiter(Process&);

            /**
             * @return true 
             * @return false 
             */
            bool unblock(Process&, WaitBlocker::UnblockFlags, u8);

            void try_unblock(WaitBlocker&);
            void finalize();

        protected:
            virtual bool should_add_blocker(Blocker&, void*) override;

        private:
            struct ProcessBlockInfo 
            {
                NonnullRefPtr<Process> process;
                WaitBlocker::UnblockFlags flags;
                u8 signal;
                bool was_waited { false };

                explicit ProcessBlockInfo(NonnullRefPtr<Process>&&, WaitBlocker::UnblockFlags, u8);
                ~ProcessBlockInfo();
            };

            Process& m_process;
            Vector<ProcessBlockInfo, 2> m_processes;
            bool m_finalized { false };
        }; // class WaitBlockCondition

        /**
         * @tparam AddBlockerHandler 
         * @param add_blocker 
         * @return KResult 
         */
        template<typename AddBlockerHandler>
        KResult try_join(AddBlockerHandler add_blocker)
        {
            if (Thread::current() == this)
                return KResult(-EDEADLK);

            ScopedSpinLock lock(m_lock);
            if (!m_is_joinable || state() == Dead)
                return KResult(-EINVAL);

            add_blocker();

            m_is_joinable = false;
            return KSuccess;
        }

        void did_schedule() { ++m_times_scheduled; }

        /**
         * @return u32 
         */
        u32 times_scheduled() const 
        { 
            return m_times_scheduled; 
        }

        void resume_from_stopped();

        bool should_be_stopped() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_stopped() const 
        { 
            return m_state == Stopped; 
        s}

        /**
         * @return true 
         * @return false 
         */
        bool is_blocked() const 
        { 
            return m_state == Blocked; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_in_block() const
        {
            ScopedSpinLock lock(m_block_lock);
            return m_in_block;
        }

        /**
         * @return u32 
         */
        u32 cpu() const 
        { 
            return m_cpu.load(Mods::MemoryOrder::memory_order_consume); 
        }

        /**
         * @param cpu 
         */
        void set_cpu(u32 cpu) 
        { 
            m_cpu.store(cpu, Mods::MemoryOrder::memory_order_release); 
        }

        /**
         * @return u32 
         */
        u32 affinity() const 
        { 
            return m_cpu_affinity; 
        }

        /**
         * @param affinity 
         */
        void set_affinity(u32 affinity) 
        { 
            m_cpu_affinity = affinity; 
        }

        /**
         * @return u32 
         */
        u32 stack_ptr() const 
        { 
            return m_tss.esp; 
        }

        /**
         * @return RegisterState& 
         */
        RegisterState& get_register_dump_from_stack();

        /**
         * @return TSS32& 
         */
        TSS32& tss() 
        { 
            return m_tss; 
        }

        /**
         * @return const TSS32& 
         */
        const TSS32& tss() const 
        { 
            return m_tss; 
        }

        /**
         * @return State 
         */
        State state() const 
        { 
            return m_state; 
        }
        
        /**
         * @return const char* 
         */
        const char* state_string() const;

        /**
         * @return u32 
         */
        u32 ticks() const 
        { 
            return m_ticks; 
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress thread_specific_data() const 
        { 
            return m_thread_specific_data; 
        }

        /**
         * @return size_t 
         */
        size_t thread_specific_region_size() const 
        { 
            return m_thread_specific_region_size; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void yield_if_stopped()
        {
            ScopedSpinLock lock(m_lock);
            while (state() == Thread::Stopped) {
                lock.unlock();
                yield_while_not_holding_big_lock();
                lock.lock();
            }
        }

        /**
         * @tparam T 
         * @tparam Args 
         * @param timeout 
         * @param args 
         * @return BlockResult 
         */
        template<typename T, class... Args>
        [[nodiscard]] BlockResult block(const BlockTimeout& timeout, Args&&... args)
        {
            ASSERT(!Processor::current().in_irq());
            ScopedCritical critical;
            ScopedSpinLock scheduler_lock(g_scheduler_lock);
            ScopedSpinLock block_lock(m_block_lock);
            m_in_block = true;
            T t(forward<Args>(args)...);

            Atomic<bool> timeout_unblocked(false);
            Atomic<bool> did_unblock(false);
            RefPtr<Timer> timer;
            {
                switch (state()) {
                case Thread::Stopped:
                    break;
                case Thread::Running:
                    ASSERT(m_blocker == nullptr);
                    break;
                default:
                    ASSERT_NOT_REACHED();
                }

                m_blocker = &t;
                if (!t.should_block()) {
                    t.not_blocking(false);
                    m_blocker = nullptr;
                    m_in_block = false;
                    return BlockResult::NotBlocked;
                }

                auto& block_timeout = t.override_timeout(timeout);
                if (!block_timeout.is_infinite()) {
                    timer = TimerQueue::the().add_timer_without_id(block_timeout.clock_id(), block_timeout.absolute_time(), [&]() {
                        ASSERT(!Processor::current().in_irq());
                        ASSERT(!g_scheduler_lock.own_lock());
                        ASSERT(!m_block_lock.own_lock());

                        ScopedSpinLock scheduler_lock(g_scheduler_lock);
                        ScopedSpinLock block_lock(m_block_lock);
                        if (m_blocker && timeout_unblocked.exchange(true, Mods::MemoryOrder::memory_order_relaxed) == false)
                            unblock();
                    });
                    if (!timer) {
                        t.not_blocking(true);
                        m_blocker = nullptr;
                        m_in_block = false;
                        return BlockResult::InterruptedByTimeout;
                    }
                }

                t.begin_blocking({});

                set_state(Thread::Blocked);
            }

            block_lock.unlock();

            bool did_timeout = false;
            bool did_unlock = false;
            for (;;) {
                scheduler_lock.unlock();

                did_unlock |= unlock_process_if_locked();

                ASSERT(!g_scheduler_lock.own_lock());
                ASSERT(Processor::current().in_critical());
                yield_while_not_holding_big_lock();

                scheduler_lock.lock();
                ScopedSpinLock block_lock2(m_block_lock);
                if (should_be_stopped() || state() == Stopped) {
                    dbg() << "Thread should be stopped this point, current state: " << state_string();
                    set_state(Thread::Blocked);
                    continue;
                }
                if (m_blocker && !m_blocker->can_be_interrupted() && !m_should_die) {
                    block_lock2.unlock();
                    dbg() << "Thread should not be unblocking, current status: " << state_string();
                    set_state(Thread::Blocked);
                    continue;
                }

                did_timeout |= timeout_unblocked.exchange(true, Mods::MemoryOrder::memory_order_relaxed);
                if (m_blocker) {
                    ASSERT(m_blocker == &t);
                    m_blocker = nullptr;
                }
                m_in_block = false;
                break;
            }

            if (t.was_interrupted_by_signal()) {
                ScopedSpinLock lock(m_lock);
                dispatch_one_pending_signal();
            }

            auto result = t.end_blocking({}, did_timeout); 

            scheduler_lock.unlock();
            if (timer && !did_timeout) {
                TimerQueue::the().cancel_timer(timer.release_nonnull());
            }
            if (did_unlock) {
                lock_process();
            }
            return result;
        }

        void unblock_from_blocker(Blocker&);
        void unblock(u8 signal = 0);

        /**
         * @return BlockResult 
         */
        BlockResult sleep(clockid_t, const timespec&, timespec* = nullptr);

        /**
         * @param duration 
         * @param remaining_time 
         * @return BlockResult 
         */
        BlockResult sleep(const timespec& duration, timespec* remaining_time = nullptr)
        {
            return sleep(CLOCK_MONOTONIC, duration, remaining_time);
        }

        /**
         * @return BlockResult 
         */
        BlockResult sleep_until(clockid_t, const timespec&);

        /**
         * @param duration 
         * @return BlockResult 
         */
        BlockResult sleep_until(const timespec& duration)
        {
            return sleep_until(CLOCK_MONOTONIC, duration);
        }

        void set_should_die();

        bool should_die() const 
        { 
            return m_should_die; 
        }

        void die_if_needed();

        void exit(void* = nullptr);

        bool tick();

        /**
         * @param t 
         */
        void set_ticks_left(u32 t) 
        { 
            m_ticks_left = t; 
        }

        /**
         * @return u32 
         */
        u32 ticks_left() const 
        { 
            return m_ticks_left; 
        }

        /**
         * @return u32 
         */
        u32 kernel_stack_base() const 
        { 
            return m_kernel_stack_base; 
        }

        /**
         * @return u32 
         */
        u32 kernel_stack_top() const 
        { 
            return m_kernel_stack_top; 
        }

        /// @brief: set state object.
        void set_state(State, u8 = 0);

        /**
         * @return true 
         * @return false 
         */
        bool is_initialized() const 
        { 
            return m_initialized; 
        }


        void set_initialized(bool initialized) 
        { 
            m_initialized = initialized; 
        }

        /**
         * @param signal 
         */
        void send_urgent_signal_to_self(u8 signal);

        /**
         * @param signal 
         * @param sender 
         */
        void send_signal(u8 signal, Process* sender);

        /**
         * @param signal_mask 
         * @return u32 
         */
        u32 update_signal_mask(u32 signal_mask);

        /**
         * @param signal_set 
         * @param block 
         * @return u32 
         */
        u32 signal_mask_block(sigset_t signal_set, bool block);

        /**
         * @return u32 
         */
        u32 signal_mask() const;

        void clear_signals();

        void set_dump_backtrace_on_finalization() 
        { 
            m_dump_backtrace_on_finalization = true; 
        }

        /**
         * @return DispatchSignalResult 
         */
        DispatchSignalResult dispatch_one_pending_signal();

        /**
         * @param signal 
         * @return DispatchSignalResult 
         */
        DispatchSignalResult try_dispatch_one_pending_signal(u8 signal);

        /**
         * 
         * @param signal 
         * @return DispatchSignalResult 
         */
        DispatchSignalResult dispatch_signal(u8 signal);

        void check_dispatch_pending_signal();
        
        /**
         * @return true 
         * @return false 
         */
        bool has_unmasked_pending_signals() const 
        { 
            return m_have_any_unmasked_pending_signals.load(Mods::memory_order_consume); 
        }

        /**
         * @param signal 
         */
        void terminate_due_to_signal(u8 signal);

        /**
         * @param signal 
         * @return true 
         * @return false 
         */
        bool should_ignore_signal(u8 signal) const;

        /**
         * @param signal 
         * @return true 
         * @return false 
         */
        bool has_signal_handler(u8 signal) const;

        /**
         * @param signal 
         * @return true 
         * @return false 
         */
        bool has_pending_signal(u8 signal) const;
        u32 pending_signals() const;

        /// @return u32 
        u32 pending_signals_for_state() const;

        /**
         * @return FPUState& 
         */
        FPUState& fpu_state() 
        { 
            return *m_fpu_state; 
        }

        void set_default_signal_dispositions();

        /**
         * @return true 
         * @return false 
         */
        bool push_value_on_stack(FlatPtr);

        /**
         * @param arguments 
         * @param environment 
         * @return KResultOr<u32> 
         */
        KResultOr<u32> make_userspace_stack_for_main_thread(Vector<String> arguments, Vector<String> environment, Vector<AuxiliaryValue>);

        /**
         * @return KResult 
         */
        KResult make_thread_specific_region(Badge<Process>);

        /**
         * @return unsigned 
         */
        unsigned syscall_count() const 
        { 
            return m_syscall_count; 
        }

        void did_syscall() 
        { 
            ++m_syscall_count; 
        }

        unsigned inode_faults() const 
        { 
            return m_inode_faults; 
        }

        void did_inode_fault() 
        { 
            ++m_inode_faults; 
        }

        /**
         * @return unsigned 
         */
        unsigned zero_faults() const 
        { 
            return m_zero_faults; 
        }

        /// @breif: did_zero_fault
        void did_zero_fault() 
        { 
            ++m_zero_faults; 
        }

        /**
         * @return unsigned 
         */
        unsigned cow_faults() const 
        { 
            return m_cow_faults; 
        }


        void did_cow_fault() 
        { 
            ++m_cow_faults; 
        }

        /**
         * @return unsigned 
         */
        unsigned file_read_bytes() const 
        { 
            return m_file_read_bytes; 
        }

        /**
         * @return unsigned 
         */
        unsigned file_write_bytes() const 
        { 
            return m_file_write_bytes; 
        }

        /**
         * @param bytes 
         */
        void did_file_read(unsigned bytes)
        {
            m_file_read_bytes += bytes;
        }

        /**
         * @param bytes 
         */
        void did_file_write(unsigned bytes)
        {
            m_file_write_bytes += bytes;
        }

        /**
         * @return unsigned 
         */
        unsigned unix_socket_read_bytes() const 
        { 
            return m_unix_socket_read_bytes; 
        }

        /**
         * @return unsigned 
         */
        unsigned unix_socket_write_bytes() const 
        { 
            return m_unix_socket_write_bytes; 
        }

        /**
         * @param bytes 
         */
        void did_unix_socket_read(unsigned bytes)
        {
            m_unix_socket_read_bytes += bytes;
        }

        /**
         * @param bytes 
         */
        void did_unix_socket_write(unsigned bytes)
        {
            m_unix_socket_write_bytes += bytes;
        }

        /**
         * @return unsigned 
         */
        unsigned ipv4_socket_read_bytes() const 
        { 
            return m_ipv4_socket_read_bytes; 
        }

        /**
         * @return unsigned 
         */
        unsigned ipv4_socket_write_bytes() const 
        { 
            return m_ipv4_socket_write_bytes; 
        }

        /**
         * @param bytes 
         */
        void did_ipv4_socket_read(unsigned bytes)
        {
            m_ipv4_socket_read_bytes += bytes;
        }

        /**
         * @param bytes 
         */
        void did_ipv4_socket_write(unsigned bytes)
        {
            m_ipv4_socket_write_bytes += bytes;
        }

        /**
         * @param active 
         */
        void set_active(bool active)
        {
            m_is_active.store(active, Mods::memory_order_release);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_active() const
        {
            return m_is_active.load(Mods::MemoryOrder::memory_order_acquire);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_finalizable() const
        {
            if (m_is_active.load(Mods::memory_order_acquire))
                return false;

            ScopedSpinLock lock(m_lock);
            return !m_is_joinable;
        }

        RefPtr<Thread> clone(Process&);

        template<typename Callback>
        static IterDecision for_each_in_state(State, Callback);

        template<typename Callback>
        static IterDecision for_each_living(Callback);
        
        template<typename Callback>
        static IterDecision for_each(Callback);

        /**
         * @param state 
         * @return true 
         * @return false 
         */
        static bool is_runnable_state(Thread::State state)
        {
            return state == Thread::State::Running || state == Thread::State::Runnable;
        }

        static constexpr u32 default_kernel_stack_size = 65536;
        static constexpr u32 default_userspace_stack_size = 4 * MiB;

        /**
         * @return RecursiveSpinLock& 
         */
        RecursiveSpinLock& get_lock() const 
        { 
            return m_lock; 
        }

    #ifdef LOCK_DEBUG
        /**
         * @param lock 
         * @param holding 
         * @param file 
         * @param line 
         */
        void holding_lock(Lock& lock, bool holding, const char* file = nullptr, int line = 0)
        {
            m_holding_locks.fetch_add(holding ? 1 : -1, Mods::MemoryOrder::memory_order_relaxed);
            ScopedSpinLock list_lock(m_holding_locks_lock);
            if (holding) {
                bool have_existing = false;
                for (size_t i = 0; i < m_holding_locks_list.size(); i++) {
                    auto& info = m_holding_locks_list[i];
                    if (info.lock == &lock) {
                        have_existing = true;
                        info.count++;
                        break;
                    }
                }
                if (!have_existing)
                    m_holding_locks_list.append({ &lock, file ? file : "unknown", line, 1 });
            } else {
                bool found = false;
                for (size_t i = 0; i < m_holding_locks_list.size(); i++) {
                    auto& info = m_holding_locks_list[i];
                    if (info.lock == &lock) {
                        ASSERT(info.count > 0);
                        if (--info.count == 0)
                            m_holding_locks_list.remove(i);
                        found = true;
                        break;
                    }
                }
                ASSERT(found);
            }
        }

        /**
         * @return u32 
         */
        u32 lock_count() const
        {
            return m_holding_locks.load(Mods::MemoryOrder::memory_order_relaxed);
        }
    #endif

    private:
        IntrusiveListNode m_runnable_list_node;

    private:
        friend struct SchedulerData;
        friend class WaitQueue;

        class JoinBlockCondition : public BlockCondition 
        {
        public:

            /**
             * @param exit_value 
             */
            void thread_did_exit(void* exit_value)
            {
                ScopedSpinLock lock(m_lock);
                ASSERT(!m_thread_did_exit);
                m_thread_did_exit = true;
                m_exit_value.store(exit_value, Mods::MemoryOrder::memory_order_release);
                do_unblock_joiner();
            }
            
            void thread_finalizing()
            {
                ScopedSpinLock lock(m_lock);
                do_unblock_joiner();
            }
    
            /*
             * @return void* 
             */
            void* exit_value() const
            {
                ASSERT(m_thread_did_exit);
                return m_exit_value.load(Mods::MemoryOrder::memory_order_acquire);
            }

            /**
             * @param blocker 
             */
            void try_unblock(JoinBlocker& blocker)
            {
                ScopedSpinLock lock(m_lock);
                if (m_thread_did_exit)
                    blocker.unblock(exit_value(), false);
            }

        protected:

            /**
             * @param b 
             * @return true 
             * @return false 
             */
            virtual bool should_add_blocker(Blocker& b, void*) override
            {
                ASSERT(b.blocker_type() == Blocker::Type::Join);
                auto& blocker = static_cast<JoinBlocker&>(b);

                if (m_thread_did_exit) {
                    blocker.unblock(exit_value(), true);
                    return false;
                }
                
                return true;
            }

        private:
            void do_unblock_joiner()
            {
                do_unblock_all([&](Blocker& b, void*) {
                    ASSERT(b.blocker_type() == Blocker::Type::Join);
                    auto& blocker = static_cast<JoinBlocker&>(b);
                    return blocker.unblock(exit_value(), false);
                });
            }

            Atomic<void*> m_exit_value { nullptr };
            bool m_thread_did_exit { false };
        }; // class JoinBlockCondition

        bool unlock_process_if_locked();
        void lock_process();
        void relock_process(bool did_unlock);
        String backtrace_impl();
        void reset_fpu_state();

        mutable RecursiveSpinLock m_lock;
        mutable RecursiveSpinLock m_block_lock;

        NonnullRefPtr<Process> m_process;
        ThreadID m_tid { -1 };

        TSS32 m_tss;
        Atomic<u32> m_cpu { 0 };

        u32 m_cpu_affinity { THREAD_AFFINITY_DEFAULT };
        u32 m_ticks { 0 };
        u32 m_ticks_left { 0 };
        u32 m_times_scheduled { 0 };
        u32 m_pending_signals { 0 };
        u32 m_signal_mask { 0 };
        u32 m_kernel_stack_base { 0 };
        u32 m_kernel_stack_top { 0 };

        OwnPtr<Region> m_kernel_stack_region;
        VirtualAddress m_thread_specific_data;
        size_t m_thread_specific_region_size { 0 };
        SignalActionData m_signal_action_data[32];
        Blocker* m_blocker { nullptr };

    #ifdef LOCK_DEBUG
        struct HoldingLockInfo 
        {
            Lock* lock;
            const char* file;
            int line;
            unsigned count;
        };
        Atomic<u32> m_holding_locks { 0 };
        SpinLock<u8> m_holding_locks_lock;
        Vector<HoldingLockInfo> m_holding_locks_list;
    #endif

        JoinBlockCondition m_join_condition;
        Atomic<bool> m_is_active { false };
        bool m_is_joinable { true };

        unsigned m_syscall_count { 0 };
        unsigned m_inode_faults { 0 };
        unsigned m_zero_faults { 0 };
        unsigned m_cow_faults { 0 };

        unsigned m_file_read_bytes { 0 };
        unsigned m_file_write_bytes { 0 };

        unsigned m_unix_socket_read_bytes { 0 };
        unsigned m_unix_socket_write_bytes { 0 };

        unsigned m_ipv4_socket_read_bytes { 0 };
        unsigned m_ipv4_socket_write_bytes { 0 };

        FPUState* m_fpu_state { nullptr };
        State m_state { Invalid };
        String m_name;
        u32 m_priority { THREAD_PRIORITY_NORMAL };
        u32 m_extra_priority { 0 };
        u32 m_priority_boost { 0 };

        State m_stop_state { Invalid };

        bool m_dump_backtrace_on_finalization { false };
        bool m_should_die { false };
        bool m_initialized { false };
        bool m_in_block { false };
        Atomic<bool> m_have_any_unmasked_pending_signals { false };

        void yield_without_holding_big_lock();
        void yield_while_not_holding_big_lock();

        /**
         * @param previous_state 
         */
        void update_state_for_thread(Thread::State previous_state);
    };

    /**
     * @tparam Callback 
     * @param callback 
     * @return IterDecision 
     */
    template<typename Callback>
    inline IterDecision Thread::for_each_living(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();

        return Thread::for_each([callback](Thread& thread) -> IterDecision {
            if (thread.state() != Thread::State::Dead && thread.state() != Thread::State::Dying)
                return callback(thread);
            return IterDecision::Continue;
        });
    }

    /**
     * @tparam Callback 
     * @param callback 
     * @return IterDecision 
     */
    template<typename Callback>
    inline IterDecision Thread::for_each(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();
        ScopedSpinLock lock(g_scheduler_lock);

        auto ret = Scheduler::for_each_runnable(callback);

        if (ret == IterDecision::Break)
            return ret;

        return Scheduler::for_each_nonrunnable(callback);
    }
    
    /**
     * @tparam Callback 
     * @param state 
     * @param callback 
     * @return IterDecision 
     */
    template<typename Callback>
    inline IterDecision Thread::for_each_in_state(State state, Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();
        ScopedSpinLock lock(g_scheduler_lock);

        auto new_callback = [=](Thread& thread) -> IterDecision {
            if (thread.state() == state)
                return callback(thread);
            return IterDecision::Continue;
        };

        if (is_runnable_state(state))
            return Scheduler::for_each_runnable(new_callback);

        return Scheduler::for_each_nonrunnable(new_callback);
    }

    /**
     * @return const LogStream& 
     */
    const LogStream& operator<<(const LogStream&, const Thread&);

    struct SchedulerData 
    {
        typedef IntrusiveList<Thread, &Thread::m_runnable_list_node> ThreadList;

        ThreadList m_runnable_threads;
        ThreadList m_nonrunnable_threads;

        /**
         * @param thread 
         * @return true 
         * @return false 
         */
        bool has_thread(Thread& thread) const
        {
            return m_runnable_threads.contains(thread) || m_nonrunnable_threads.contains(thread);
        }

        /**
         * @param state 
         * @return ThreadList& 
         */
        ThreadList& thread_list_for_state(Thread::State state)
        {
            if (Thread::is_runnable_state(state))
                return m_runnable_threads;
            return m_nonrunnable_threads;
        }
    };

    /**
     * @tparam Callback 
     * @param callback 
     * @return IterDecision 
     */
    template<typename Callback>
    inline IterDecision Scheduler::for_each_runnable(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();
        ASSERT(g_scheduler_lock.own_lock());

        auto& tl = g_scheduler_data->m_runnable_threads;

        for (auto it = tl.begin(); it != tl.end();) {
            auto& thread = *it;
            it = ++it;
            if (callback(thread) == IterDecision::Break)
                return IterDecision::Break;
        }

        return IterDecision::Continue;
    }

    /**
     * @tparam Callback 
     * @param callback 
     * @return IterDecision 
     */
    template<typename Callback>
    inline IterDecision Scheduler::for_each_nonrunnable(Callback callback)
    {
        ASSERT_INTERRUPTS_DISABLED();
        ASSERT(g_scheduler_lock.own_lock());

        auto& tl = g_scheduler_data->m_nonrunnable_threads;

        for (auto it = tl.begin(); it != tl.end();) {
            auto& thread = *it;
            it = ++it;
            if (callback(thread) == IterDecision::Break)
                return IterDecision::Break;
        }

        return IterDecision::Continue;
    }

} // namespace Kernel