/**
 * @file queue.h
 * @author Krisna Pranav
 * @brief queue
 * @version 6.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "singlelinkedlist.h"
#include "vector.h"
#include "ownptr.h"

namespace Mods
{

    template<typename T, int segment_size = 1000>
    class Queue 
    {
    public:
        Queue() { }
        ~Queue() { }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return m_size == 0; 
        }

        /**
         * @param value 
         */
        void enqueue(T&& value)
        {
            if (m_segments.is_empty() || m_segments.last()->size() >= segment_size)
                m_segments.append(make<Vector<T, segment_size>>());
            m_segments.last()->append(move(value));
            ++m_size;
        }

        /**
         * @param value 
         */
        void enqueue(const T& value)
        {
            enqueue(T(value));
        }

        /**
         * @return T 
         */
        T dequeue()
        {
            ASSERT(!is_empty());
            auto value = move((*m_segments.first())[m_index_into_first++]);
            if (m_index_into_first == segment_size) {
                m_segments.take_first();
                m_index_into_first = 0;
            }
            --m_size;
            return value;
        }

        /**
         * @return const T& 
         */
        const T& head() const
        {
            ASSERT(!is_empty());
            return (*m_segments.first())[m_index_into_first];
        }

        /// @breif: clear[segments, index_into_first, size]
        void clear()
        {
            m_segments.clear();
            m_index_into_first = 0;
            m_size = 0;
        }

    private:
        SinglyLinkedList<OwnPtr<Vector<T, segment_size>>> m_segments;
        size_t m_index_into_first { 0 };
        size_t m_size { 0 };
    }; // class Queue

} // namespace Mods

using Mods::Queue;