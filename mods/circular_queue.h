/**
 * @file circular_queue.h
 * @author Krisna Pranav
 * @brief CircularQueue
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        CircularQueue()
        { }

        ~CircularQueue() {
            clear();
        }

        void clear() {
            for (size_t i = 0; i < m_size; ++i)
                elements()[(m_head + i) % Capacity].~T();

            m_head = 0;
            m_size = 0;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return !m_size; 
        }

        /**
         * @return size_t 
         */
        size_t size() const { 
            return m_size; 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const { 
            return Capacity; 
        }

        /**
         * @param value 
         */
        void enqueue(T&& value) {
            auto& slot = elements()[(m_head + m_size) % Capacity];
            if (m_size == Capacity)
                slot.~T();

            new (&slot) T(move(value));
            if (m_size == Capacity)
                m_head = (m_head + 1) % Capacity;
            else
                ++m_size;
        }

        /**
         * @param value 
         */
        void enqueue(const T& value) {
            enqueue(T(value));
        }

        /**
         * @return T 
         */
        T dequeue() {
            ASSERT(!is_empty());
            auto& slot = elements()[m_head];
            T value = move(slot);
            slot.~T();
            m_head = (m_head + 1) % Capacity;
            --m_size;
            return value;
        }

        /**
         * @param index 
         * @return const T& 
         */
        const T& at(size_t index) const { 
            return elements()[(m_head + index) % Capacity]; 
        }

        /**
         * @return const T& 
         */
        const T& first() const { 
            return at(0); 
        }

        /**
         * @return const T& 
         */
        const T& last() const { 
            return at(size() - 1); 
        }

        class ConstIterator {
        public:

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const ConstIterator& other) { 
                return m_index != other.m_index; 
            }

            /**
             * @return ConstIterator& 
             */
            ConstIterator& operator++() {
                m_index = (m_index + 1) % Capacity;
                if (m_index == m_queue.m_head)
                    m_index = m_queue.m_size;
                return *this;
            }

            /**
             * @return const T& 
             */
            const T& operator*() const { 
                return m_queue.elements()[m_index]; 
            }

        private:
            friend class CircularQueue;

            /**
             * @param queue 
             * @param index 
             */
            ConstIterator(const CircularQueue& queue, const size_t index)
                : m_queue(queue)
                , m_index(index)
            {
            }
            
            const CircularQueue& m_queue;
            size_t m_index { 0 };
        };

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const { 
            return ConstIterator(*this, m_head); 
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const { 
            return ConstIterator(*this, size()); 
        }

        /**
         * @return size_t 
         */
        size_t head_index() const { 
            return m_head; 
        }

    protected:

        /**
         * @return T* 
         */
        T* elements() { 
            return reinterpret_cast<T*>(m_storage); 
        }

        /**
         * @return const T* 
         */
        const T* elements() const { 
            return reinterpret_cast<const T*>(m_storage); 
        }

        friend class ConstIterator;

        alignas(T) u8 m_storage[sizeof(T) * Capacity];
        
        size_t m_size { 0 };
        size_t m_head { 0 };
    };

}

using Mods::CircularQueue;
