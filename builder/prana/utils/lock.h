#pragma once

#include <atomic>

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
};

class SpinLock {
public:
    SpinLock() = default;

    void lock() {
        while (m_locked.exchange(true)) {}
    }

    void unlock() {
        m_locked.store(false);
    }
}