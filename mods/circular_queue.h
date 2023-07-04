/**
 * @file circular_queue.h
 * @author Krisna Pranav
 * @brief CircularQueue
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "stdlibextra.h"
#include "noncopyable.h"
#include "assertions.h"
#include "forward.h"

namespace Mods {
    template<typename T, size_t Capacity>
    class CircularQueue {
        friend CircularDuplexStream<Capacity>;
    
    public:
        CircularQueue() {}

        ~CircularQueue() {
            clear();
        }

        void clear() {
            for (size_t i = 0; i < m_size; ++i)
                elements()[(m_head + i) % Capacity].~T();
            
            m_head = 0;
            m_size = 0;
        }

        bool is_empty() const {
            return !m_size;
        }

        size_t size() const {
            return m_size;
        }

        size_t capacity() const {
            return Capacity;
        }
        
        void enqueue(T&& value) {
            auto& slot == elements()[(m_head)];
        }
    
    protected:
        T* elements() {
            return reinterpret_cast<T*>(m_storage);
        }

        friend class ConstIterator;

        size_t m_size { 0 };
        size_t m_head { 0 };

        
    }
}