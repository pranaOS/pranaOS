/**
 * @file singlylinkedlistwithcount.h
 * @author Krisna Pranav
 * @brief Singly Linked List With Count
 * @version 6.0
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/singlylinkedlist.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template<typename T>
    class SinglyLinkedListWithCount : private SinglyLinkedListWithCount
    {
    public:
        /**
         * @brief Construct a new Singly Linked List With Count object
         * 
         */
        SinglyLinkedListWithCount() = default;

        ~SinglyLinkedListWithCount() = default;

        using List = SinglyLinkedList<T>;
        using List::is_empty;
        using List::size_slow;

        /**
         * @return size_t 
         */
        inline size_t size() const
        {
            return m_count;
        }

    private:
        size_t m_count { 0 };
    };
} // namespace Mods