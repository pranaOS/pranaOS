/**
 * @file queue.h
 * @author Krisna Pranav
 * @brief queue
 * @version 6.0
 * @date 2023-07-30
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/intrusivelist.h>
#include <mods/ownPtr.h>
#include <mods/singlylinkedlist.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @tparam T 
     * @tparam segment_size 
     */
    template <typename T, int segment_size = 1000>
    class Queue
    {
    public:
        /**
         * @brief Construct a new Queue object
         * 
         */
        Queue() = default;

        /**
         * @brief Destroy the Queue object
         * 
         */
        ~Queue()
        {
            clear();
        }

        size_t size() const
        {
            return m_size;
        }

        bool is_empty() const
        {
            return m_size == 0;
        }

        /**
         * @tparam U 
         * @param value 
         */
        template <typename U = T>
        void enqueue(U&& value)
        {
            if(m_segments.is_empty() || m_segments.last()->data.size() >= segment_size)
            {
                auto segment = new QueueSegment;
                m_segments.append(*segment);
            }
            m_segments.last()->data.append(forward<U>(value));
            ++m_size;
        }

        /**
         * @return T 
         */
        T dequeue()
        {
            VERIFY(!is_empty());
            auto value = move(m_segments.first()->data[m_index_into_first++]);

            if(m_index_into_first == segment_size)
            {
                delete m_segments.take_first();
                m_index_into_first = 0;
            }

            --m_size;

            if(m_size == 0 && !m_segments.is_empty())
            {
                m_index_into_first = 0;
                m_segments.last()->data.clear_with_capacity();
            }
            return value;
        }

        /**
         * @return const T& 
         */
        const T& head() const
        {
            VERIFY(!is_empty());
            return m_segments.first()->data[m_index_into_first];
        }

        void clear()
        {
            while(auto* segment = m_segments.take_first())
                delete segment;
            m_index_into_first = 0;
            m_size = 0;
        }

    private:
        struct QueueSegment
        {
            Vector<T, segment_size> data;
            IntrusiveListNode<QueueSegment> node;
        }; // struct QueueSegment

        IntrusiveList<&QueueSegment::node> m_segments;
        size_t m_index_into_first{0};
        size_t m_size{0};
    }; // class Queue
} // namespace Mods

using Mods::Queue;