/**
 * @file circular_queue.h
 * @author Krisna Pranav
 * @brief CircularQueue
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/forward.h>
#include <mods/stdlibextra.h>

namespace Mods 
{
    /**
     * @tparam T 
     * @tparam Capacity 
     */
    template<typename T, size_t Capacity>
    class CircularQueue 
    {
        friend CircularDuplexStream<Capacity>;

    public:
        /**
         * @brief Construct a new Circular Queue object
         * 
         */
        CircularQueue() = default;

        /**
         * @brief Destroy the Circular Queue object
         * 
         */
        ~CircularQueue()
        {
            clear();
        }
        
        void clear()
        {
            for (size_t i = 0; i < m_size; ++i)
                elements()[(m_head + i) % Capacity].~T();

            m_head = 0;
            m_size = 0;
        }

        bool is_empty() const 
        { 
            return m_size == 0; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const 
        { 
            return Capacity; 
        }

        /**
         * @tparam U 
         * @param value 
         */
        template<typename U = T>
        void enqueue(U&& value)
        {
            auto& slot = elements()[(m_head + m_size) % Capacity];

            if (m_size == Capacity)
                slot.~T();

            new (&slot) T(forward<U>(value));

            if (m_size == Capacity)
                m_head = (m_head + 1) % Capacity;
            else
                ++m_size;
        }

        /**
         * @return T 
         */
        T dequeue()
        {
            VERIFY(!is_empty());
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
        const T& at(size_t index) const 
        { 
            return elements()[(m_head + index) % Capacity]; 
        }

        /**
         * @param index 
         * @return T& 
         */
        T& at(size_t index) 
        { 
            return elements()[(m_head + index) % Capacity]; 
        }

        /**
         * @return const T& 
         */
        const T& first() const 
        { 
            return at(0); 
        }

        /**
         * @return const T& 
         */
        const T& last() const 
        { 
            return at(size() - 1); 
        }

        class ConstIterator 
        {
        public:
            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(ConstIterator const& other) 
            { 
                return m_index != other.m_index; 
            }

            /**
             * @return ConstIterator& 
             */
            ConstIterator& operator++()
            {
                ++m_index;
                return *this;
            }

            /**
             * @return const T& 
             */
            const T& operator*() const 
            { 
                return m_queue.at(m_index); 
            }

        private:
            friend class CircularQueue;

            /**
             * @brief Construct a new Const Iterator object
             * 
             * @param queue 
             * @param index 
             */
            ConstIterator(CircularQueue const& queue, const size_t index)
                : m_queue(queue)
                , m_index(index)
            {
            }

            CircularQueue const& m_queue;
            size_t m_index { 0 };
        }; // class ConstIterator 

        class Iterator 
        {
        public:
            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(Iterator const& other) 
            { 
                return m_index != other.m_index; 
            }

            /**
             * @return Iterator& 
             */
            Iterator& operator++()
            {
                ++m_index;
                return *this;
            }

            /**
             * @return T& 
             */
            T& operator*() const 
            { 
                return m_queue.at(m_index); 
            }

        private:
            friend class CircularQueue;

            /**
             * @brief Construct a new Iterator object
             * 
             * @param queue 
             * @param index 
             */
            Iterator(CircularQueue& queue, size_t const index)
                : m_queue(queue)
                , m_index(index)
            {
            }

            CircularQueue& m_queue;
            size_t m_index { 0 };
        }; // class Iterator 

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const 
        { 
            return ConstIterator(*this, 0);
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const 
        { 
            return ConstIterator(*this, size()); 
        }

        /**
         * @return Iterator 
         */
        Iterator begin() 
        { 
            return Iterator(*this, 0); 
        }

        /**
         * @return Iterator 
         */
        Iterator end() 
        { 
            return Iterator(*this, size()); 
        }

        /**
         * @return size_t 
         */
        size_t head_index() const 
        { 
            return m_head; 
        }

    protected:
        /**
         * @return T* 
         */
        T* elements() 
        { 
            return reinterpret_cast<T*>(m_storage); 
        }

        /**
         * @return const T* 
         */
        const T* elements() const 
        { 
            return reinterpret_cast<const T*>(m_storage); 
        }

        friend class ConstIterator;
        alignas(T) u8 m_storage[sizeof(T) * Capacity];
        size_t m_size { 0 };
        size_t m_head { 0 };
    }; // class CircularQueue
} // namespace Mods

using Mods::CircularQueue;
