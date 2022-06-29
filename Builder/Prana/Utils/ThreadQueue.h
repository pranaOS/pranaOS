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

public:
    void enqueue(const ValueType& value);
    bool dequeue(ValueType& value);
    void enqueue(ValueType&& value);

private:
    SpinLock m_lock {};
    std::queue<ValueType> m_queue;
};

template <typename ValueType>
void ThreadQueue<ValueType>::enqueue(const ValueType& value)
{
    enqueu(ValueType(value));
}