/**
 * @file singlelinkedlist.h
 * @author Krisna Pranav
 * @brief Single Linked List
 * @version 1.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "forward.h"
#include "traits.h"
#include "types.h"
#include "stdlibextra.h"

namespace Mods
{

    template <typename ListType, typename ElementType>
    class SinglyLinkedListIterator
    {
    public:
        SinglyLinkedListIterator() {}

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const SinglyLinkedListIterator& other) const
        {
            return m_node != other.m_node;
        }

        /**
         * @return SinglyLinkedListIterator& 
         */
        SinglyLinkedListIterator& operator++()
        {
            m_prev = m_node;
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

        /**
         * @return true 
         * @return false 
         */
        bool is_end() const
        {
            return !m_node;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_begin() const
        {
            return !m_prev;
        }

    private:
        friend ListType;

        /**
         * @param node 
         * @param prev 
         */
        explicit SinglyLinkedListIterator(typename ListType::Node* node, typename ListType::Node* prev = nullptr) : m_node(node), m_prev(prev)
        {}

        typename ListType::Node* m_node { nullptr };
        typename ListType::Node* m_prev { nullptr };
    }; // class SinglyLinkedListIterator

    template<typename T>
    class SinglyLinkedList
    {
    private:
        struct Node
        {
            explicit Node(T&& v)
                : value(move(v))
            {}
        };

    public:
        SinglyLinkedList() {}

        ~SinglyLinkedList() 
        {
            clear();
        }

        void clear()
        {
           for (auto* node = m_head; node;) {
                auto* next = node->next;
                delete node;
                node = next;
           } 

           m_head = nullptr;
           m_tail = nullptr;

        }

    private:
        Node* head() 
        {
            return m_head;
        }

        const Node* head() const
        {
            return m_head;
        }

        Node* tail() 
        {
            return m_tail;
        }

        const Node* tail() const
        {
            return m_tail;
        }


        Node* m_head { nullptr };
        Node* m_tail { nullptr };
    } // class SinglyLinkedList

} // namespace Mods 