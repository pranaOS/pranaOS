/**
 * @file singlelinkedlistcount.h
 * @author Krisna Pranav
 * @brief SingleLinkedListCount
 * @version 6.0
 * @date 2023-07-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/singlylinkedlist.h>

namespace Mods
{
    template<typename T>
    class SinglyLinkedListCount: private SinglyLinkedList<T>
    {

    public:
        SinglyLinkedListCount() {}
        ~SinglyLinkedListCount() {}

        using List = SinglyLinkedList<T>;

        /// @brief: is_empty + size_slow
        using List::is_empty;
        using List::size_slow;

        /**
         * @return size_t 
         */
        inline size_t size() const
        {
            return m_count;
        }

        /**
         */
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
         * @param value 
         */
        void append(const T& value)
        {
            m_count++;

            return SinglyLinkedList<T>::apend(value);
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

        /**
         * @tparam Finder 
         * @param finder 
         * @return ConstIterator 
         */
        template<typename Finder>
        ConstIterator find(Finder finder) const
        {
            return List::find(finder);
        }   

        /**
         * @tparam Finder 
         * @param finder 
         * @return Iterator 
         */
        template<typename Finder>
        Iterator find(Finder finder)
        {
            return List::find(finder);
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const
        {
            return List::find(value);
        }

    private:
        size_t m_count { 0 };

    }; // class SinglyLinkedListCount
}