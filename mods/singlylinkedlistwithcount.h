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
    template <typename T>
    class SinglyLinkedListWithCount : private SinglyLinkedList<T>
    {

    public: 
        /**
         * @brief Construct a new SinglyLinkedListWithCount object
         * 
         */
        SinglyLinkedListWithCount() = default;

        /**
         * @brief Destroy the SinglyLinkedListWithCount object
         * 
         */
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

        void clear()
        {
            List::clear();
            m_count = 0;
        }

        /**
         * @return T& 
         */
        T& first()
        {
            return List::first();
        }

        /**
         * @return const T& 
         */
        const T& first() const
        {
            return List::first();
        }

        /**
         * @return T& 
         */
        T& last()
        {
            return List::last();
        }

        /**
         * @return const T& 
         */
        const T& last() const
        {
            return List::last();
        }

        /**
         * @return T 
         */
        T take_first()
        {
            m_count--;
            return List::take_first();
        }

        /**
         * @tparam U 
         * @param value 
         */
        template <typename U = T>
        void append(U&& value)
        {
            m_count++;
            return List::append(forward<T>(value));
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(const T& value) const
        {
            return List::contains_slow(value);
        }

        using Iterator = typename List::Iterator;
        friend Iterator;

        /**
         * @return Iterator 
         */
        Iterator begin()
        {
            return List::begin();
        }

        /**
         * @return Iterator 
         */
        Iterator end()
        {
            return List::end();
        }

        using ConstIterator = typename List::ConstIterator;
        friend ConstIterator;

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const
        {
            return List::begin();
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const
        {
            return List::end();
        }

        /**
         * @tparam TUnaryPredicate 
         * @param pred 
         * @return ConstIterator 
         */
        template <typename TUnaryPredicate>
        ConstIterator find(TUnaryPredicate&& pred) const
        {
            return List::find_if(forward<TUnaryPredicate>(pred));
        }

        /**
         * @tparam TUnaryPredicate 
         * @param pred 
         * @return Iterator 
         */
        template <typename TUnaryPredicate>
        Iterator find(TUnaryPredicate&& pred)
        {
            return List::find_if(forward<TUnaryPredicate>(pred));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const
        {
            return List::find(value);
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value)
        {
            return List::find(value);
        }

        /**
         * @param iterator 
         */
        void remove(Iterator iterator)
        {
            m_count--;
            return List::remove(iterator);
        }

        /**
         * @tparam U 
         * @param iterator 
         * @param value 
         */
        template <typename U = T>
        void insert_before(Iterator iterator, U&& value)
        {
            m_count++;
            List::insert_before(iterator, forward<T>(value));
        }

        /**
         * @tparam U 
         * @param iterator 
         * @param value 
         */
        template <typename U = T>
        void insert_after(Iterator iterator, U&& value)
        {
            m_count++;
            List::insert_after(iterator, forward<T>(value));
        }

    private:
        size_t m_count{0};
    }; // class SinglyLinkedListWithCount : private SinglyLinkedList<T>
} // namespace Mods

using Mods::SinglyLinkedListWithCount;