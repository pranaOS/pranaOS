/**
 * @file circular_deque.h
 * @author Krisna Pranav
 * @brief Circular Dequeue
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/circularqueue.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     * @tparam Capacity 
     */
    template<typename T, size_t Capacity>
    class CircularDeque : public CircularQueue<T, Capacity> 
    {
    public:
        /**
         * @tparam U 
         * @param value 
         */
        template<typename U = T>
        void enqueue_begin(U&& value)
        {
            auto const new_head = (this->m_head - 1 + Capacity) % Capacity;
            auto& slot = this->elements()[new_head];

            if (this->m_size == Capacity)
                slot.~T();

            else
                ++this->m_size;

            new (&slot) T(forward<U>(value));
            this->m_head = new_head;
        }

        /**
         * @return T 
         */
        T dequeue_end()
        {
            VERIFY(!this->is_empty());
            auto& slot = this->elements()[(this->m_head + this->m_size - 1) % Capacity];
            T value = move(slot);
            slot.~T();
            this->m_size--;
            return value;
        }
    }; // class CircularDeque : public CircularQueue<T, Capacity> 
} // namespace Mods

using Mods::CircularDeque;