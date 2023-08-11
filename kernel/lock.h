/**
 * @file lock.h
 * @author Krisna Pranav
 * @brief lock
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/arch/i386/cpu.h>
#include <kernel/forward.h>
#include <kernel/thread.h>
#include <kernel/waitqueue.h>
#include <mods/atomic.h>
#include <mods/types.h>
#include <mods/assertions.h>

namespace Kernel
{
    class Lock
    {
    public:
        Lock(const char* name = nullptr)
            : m_name(name)
        {}

        ~Lock() {}

        enum class Mode
        {
            Unlocked,
            Shared,
            Exclusive
        };

        void lock(Mode = Mode::Exclusive);

        #ifdef LOCK_DEBUG
            void lock(const char* file, int line, Mode mode = Mode::Exclusive);
        #endif

        void unlock();

        bool force_unlock_if_locked();

        /**
         * @return true 
         * @return false 
         */
        bool is_locked() const
        {
            return m_holder;
        }

        void clear_waiters();

        /**
         * @return const char* 
         */
        const char* name() const
        {
            return m_name;
        }

    private:
        Atomic<bool> m_lock { false };
        const char* m_name { nullptr };

        WaitQueue m_queue;

        Mode m_mode { Mode::Unlocked };

        u32 m_times_locked { 0 };

        RefPtr<Thread> m_holder;
    }; // class Lock

    class Locker
    {
    public:
        #ifdef LOCK_DEBUG
            /**
             * @param file 
             * @param line 
             * @param l 
             * @param mode 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE explicit Locker(const char* file, int line, Lock& l, Lock::Mode mode = Lock::Mode::Exclusive)
                : m_lock(l)
            {
                m_lock.lock(mode);
            }
        #endif 

        /**
         * @param l 
         * @param mode 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit Locker(Lock& l, Lock::Mode mode = Lock::Mode::Exclusive)
            : m_lock(l)
        {
            m_lock.lock(mode);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~Locker() 
        {
            unlock();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unlock()
        {
            m_lock.unlock();
        }

        /**
         * @param mode 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void lock(Lock::Mode mode = Lock::Mode::Exclusive)
        {
            m_lock.lock(mode);
        }

    private:
        Lock& m_lock;
    }; // class Locker

} // namespace Kernel