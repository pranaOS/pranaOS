/**
 * @file mutex.h
 * @author Krisna Pranav
 * @brief Mutex
 * @version 6.0
 * @date 2024-10-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/hashmap.h>
#include <mods/types.h>
#include <kernel/forward.h>
#include <kernel/locking/locklocation.h>
#include <kernel/locking/lockmode.h>
#include <kernel/waitqueue.h>

namespace Kernel 
{

    class Mutex 
    {
        friend class Thread;

        MOD_MAKE_NONCOPYABLE(Mutex);
        MOD_MAKE_NONMOVABLE(Mutex);

    public:
        using Mode = LockMode;

        /**
         * @brief Construct a new Mutex object
         * 
         * @param name 
         */
        Mutex(StringView name = {})
            : m_name(name)
        { }

        /**
         * @brief Destroy the Mutex object
         * 
         */
        ~Mutex() = default;

        /**
         * @brief 
         * 
         * @param mode 
         * @param location 
         */
        void lock(Mode mode = Mode::Exclusive, LockLocation const& location = LockLocation::current());

        /**
         * @param location 
         */
        void restore_exclusive_lock(u32, LockLocation const& location = LockLocation::current());

        void unlock();

        /**
         * @return Mode 
         */
        [[nodiscard]] Mode force_unlock_exclusive_if_locked(u32&);

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_locked() const
        {
            SpinlockLocker lock(m_lock);
            return m_mode != Mode::Unlocked;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_exclusively_locked_by_current_thread() const
        {
            SpinlockLocker lock(m_lock);
            VERIFY(m_mode != Mode::Shared); 
            if (m_mode == Mode::Unlocked)
                return false;
            return m_holder == Thread::current();
        }

        /**
         * @return StringView 
         */
        [[nodiscard]] StringView name() const 
        { 
            return m_name; 
        }

        /**
         * @param mode 
         * @return StringView 
         */
        static StringView mode_to_string(Mode mode)
        {
            switch (mode) {
            case Mode::Unlocked:
                return "unlocked"sv;
            case Mode::Exclusive:
                return "exclusive"sv;
            case Mode::Shared:
                return "shared"sv;
            default:
                return "invalid"sv;
            }
        }

    private:
        using BlockedThreadList = IntrusiveList<&Thread::m_blocked_threads_list_node>;

        /**
         * @param mode 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE BlockedThreadList& thread_list_for_mode(Mode mode)
        {
            VERIFY(mode == Mode::Exclusive || mode == Mode::Shared);
            return mode == Mode::Exclusive ? m_blocked_threads_list_exclusive : m_blocked_threads_list_shared;
        }

        void block(Thread&, Mode, SpinlockLocker<Spinlock>&, u32);
        void unblock_waiters(Mode);

        StringView m_name;
        Mode m_mode { Mode::Unlocked };

        u32 m_times_locked { 0 };

        RefPtr<Thread> m_holder;
        size_t m_shared_holders { 0 };

        BlockedThreadList m_blocked_threads_list_exclusive;
        BlockedThreadList m_blocked_threads_list_shared;

        mutable Spinlock m_lock;

    #if LOCK_SHARED_UPGRADE_DEBUG
        HashMap<Thread*, u32> m_shared_holders_map;
    #endif
    }; // class Mutex

    class MutexLocker 
    {
        MOD_MAKE_NONCOPYABLE(MutexLocker);

    public:

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit MutexLocker()
            : m_lock(nullptr)
            , m_locked(false)
        { }

        /**
         * @param l 
         * @param mode 
         * @param location 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit MutexLocker(Mutex& l, Mutex::Mode mode = Mutex::Mode::Exclusive, LockLocation const& location = LockLocation::current())
            : m_lock(&l)
        {
            m_lock->lock(mode, location);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~MutexLocker()
        {
            if (m_locked)
                unlock();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock()
        {
            VERIFY(m_lock);
            VERIFY(m_locked);
            m_locked = false;
            m_lock->unlock();
        }

        /**
         * @param lock 
         * @param mode 
         * @param location 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void attach_and_lock(Mutex& lock, Mutex::Mode mode = Mutex::Mode::Exclusive, LockLocation const& location = LockLocation::current())
        {
            VERIFY(!m_locked);
            m_lock = &lock;
            m_locked = true;

            m_lock->lock(mode, location);
        }

        /**
         * @param mode 
         * @param location 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void lock(Mutex::Mode mode = Mutex::Mode::Exclusive, LockLocation const& location = LockLocation::current())
        {
            VERIFY(m_lock);
            VERIFY(!m_locked);
            m_locked = true;

            m_lock->lock(mode, location);
        }

    private:
        Mutex* m_lock;
        bool m_locked { true };
    }; // class MutexLocker

} // namespace Kernel