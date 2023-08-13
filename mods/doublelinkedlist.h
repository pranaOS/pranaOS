/**
 * @file doublelinkedlist.h
 * @author Krisna Pranav
 * @brief double linked list
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/stdlibextra.h>
#include <mods/traits.h>

namespace Mods 
{
    template<typename ListType, typename ElementType>
    class DoubleLinkedListIterator
    {
    public:
        bool operator!=(const DoubleLinkedListIterator& other) const
        {
            return m_node != other.m_node;
        } 

        bool operator==(const DoubleLinkedListIterator& other) const
        {
            return m_node == other.m_node;
        }

        /**
         * @return DoubleLinkedListIterator& 
         */
        DoubleLinkedListIterator& operator++()
        {
            m_node = m_node->next;
            return *this;
        }

        /**
         * @return ElementType& 
         */
        ElementType& operator*()
        {
            return m_node->value;
        }

        /**
         * @return ElementType* 
         */
        ElementType* operator->()
        {
            return &m_node->value;
        }

    private:
        friend ListType;

        explicit DoubleLinkedListIterator(typename ListType::Node* node)
            : m_node(node)
        {}

        typename ListType::Node* m_node;

    }; // class DoubleLinkedListIterator

    template<typename T>
    class DoubleLinkedList
    {
        
    }; // class DoubleLinkedList

} // namespace Mods 