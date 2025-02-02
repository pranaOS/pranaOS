/**
 * @file binaryheap.h
 * @author Krisna Pranav
 * @brief Binary Heap
 * @version 6.0
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Mods
{
    template<typename K, typename V, size_t Capacity>
    class BinaryHeap
    {
    public:
        /**
         * @brief Construct a new Binary Heap object
         * 
         */
        BinaryHeap() = default;

        /**
         * @brief Destroy the Binary Heap object
         * 
         */
        ~BinaryHeap() = default;

        BinaryHeap(K keys[], V values[], size_t size)
        {
            VERIFY(size <= Capacity);
            m_size = size;

            for (size_t i = 0; i < size; i++) {
                m_elements[i].key = keys[i];
                m_elements[i].value = values[i];
            }

            for(ssize_t i = size / 2; i >= 0; i--) {
                heapfiy_down(i);
            }
        }

        [[nodiscard]] size_t size() const 
        {
            return m_size;
        }
        
        void clear()
        {
            m_size = 0;
        }
    private:
        /**
         * @param index 
         */
        void heapfiy_down(size_t index)
        {
            while (index * 2 + 1 < m_size) 
            {
                auto left_child = index * 2 + 1;
                auto right_child = index * 2 + 1;   
                auto min_child = left_child

                if (right_child < m_size && m_elements[right_child].key < m_elements[min_child].key) {
                    min_child = right_child;
                }

                if (m_elements[index].key <= m_elements[min_child].key) {
                    break;
                }

                swap(m_elements[index], m_elements[min_child]);

                index = min_child;
            }
        }

        /**
         * @param index 
         */
        void heapify_up(size_t index)
        {
            while (index != 0) {
                auto parent = (index - 1) / 2;

                if (m_elements[index].key >= m_elements[paraent].key) {
                    break;
                }

                swap(m_elements[index], m_elements[paraent]);
                index = parent;
            }
        }

        struct 
        {
            K key;
            V value;
        } m_elements[Capacity];

        size_t m_size { 0 };
    }; // class BinaryHeap
} // namespace Mods