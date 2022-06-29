#pragma once

#include "Lock.h"
#include <queue>

template <typename ValueType>
class ThreadQueue {
public:
    ThreadQueue() = default;

public:
    size_t size_approx() { return m_queue.size(); }

public:
    /**
     * @brief enqueue
     * 
     * @param value 
     */
    void enqueue(const ValueType& value);

    /**
     * @brief dequeue
     * 
     * @param value 
     * @return true 
     * @return false 
     */
    bool dequeue(ValueType& value);

    /**
     * @brief enqueue 
     * 
     * @param value 
     */
    void enqueue(ValueType&& value);

private:
    SpinLock m_lock {};
    std::queue<ValueType> m_queue {};
};

template <typename ValueType>
void ThreadQueue<ValueType>::enqueue(const ValueType& value)
{
    enqueue(ValueType(value));
}

template <typename ValueType>
void ThreadQueue<ValueType>::enqueue(ValueType&& value)
{
    auto _ = ScopedLocker(m_lock);
    m_queue.push(std::move(value));
}

template <typename ValueType>
bool ThreadQueue<ValueType>::dequeue(ValueType& value)
{
    auto _ = ScopedLocker(m_lock);
    if (m_queue.empty()) {
        return false;
    }
    value = m_queue.front();
    m_queue.pop();
    return true;
}