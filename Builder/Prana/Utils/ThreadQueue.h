#pragma once

#include "Lock.h"
#include <queue>

template <typename ValueType>
class ThreadQueue {
public:
    ThreadQueue() = default;

public:
    size_t size_approx() {
        return m_queue.size();
    }

private:
    SpinLock m_lock {};
    std::queue<ValueType> m_queue;
};