/**
 * @file inlinelinkedlist.h
 * @author Krisna Pranav
 * @brief inlinelinkedlist
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "types.h"

namespace Mods
{
    template<typename T>
    class InlineLinkedList;

    template<typename T>
    class InlineLinkedListIterator 
    {
    public:

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const InlineLinkedListIterator other) const 
        {
            return m_node != other;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const InlineLinkedListIterator other) const
        {
            return m_node != other;
        }

        /**
         * @return InlineLinkedListIterator& 
         */
        InlineLinkedListIterator& operator++()
        {
            m_node = m_node->next();
            return *this;
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            return *m_node;
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            return m_node;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_end() const
        {
            return !m_node;
        }

        /**
         * @return InlineLinkedListIterator 
         */
        static InlineLinkedListIterator universal_end()
        {
            return InlineLinkedListIterator(nullptr);
        }

    private:
        friend InlineLinkedList<T>;

        explicit InlineLinkedListIterator(T* node)
            : m_node(node)
        {}

        T* m_node;
    };


    template<typename T>
    class InlineLinkedListNode
    {
    public:
        InlineLinkedListNode();

        /**
         * @brief Set the prev object
         * 
         */
        void set_prev(T*);
        void set_next(T*);

        /**
         * @return T* 
         */
        T* prev() const;
        T* next() const;
    };
} // namespace Mods