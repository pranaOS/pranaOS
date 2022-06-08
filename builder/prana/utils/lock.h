#pragma once

#include <atomic>

/**
 * @brief ScopedLocker
 * 
 * @tparam Lock 
 */
template <class Lock>
class ScopedLocker {
public:
    explicit ScopedLocker(Lock& lock)
        : m_lock(lock)
    {
        m_lock.lock();
    }

    ~ScopedLocker() {
        m_lock.unlock();
    }

private:
    Lock& m_lock;
};

/**
 * @brief Spin Lock
 * 
 */
class SpinLock {
public:
    SpinLock() = default;

    void lock() {
        while (m_locked.exchange(true)) { }
    }

    void unlock() {
        m_locked.store(false);
    }

private:
    std::atomic<bool> m_locked {};
};