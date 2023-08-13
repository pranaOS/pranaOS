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

        bool is_end() const
        {
            return !m_node;
        }

        /**
         * @return DoubleLinkedListIterator 
         */
        static DoubleLinkedListIterator universal_end()
        {
            return DoubleLinkedListIterator(nullptr);
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
    private:
        struct Node
        {
            explicit Node(const T& v)
                : value(v)
            {}

            explicit Node(T&& v)
                : value(move(v))
            {}

            T value;
            Node* next { nullptr };
            Node* prev { nullptr };
        }; // struct Node

    public:
        DoubleLinkedList() {}

        ~DoubleLinkedList() { clear(); }

        

    private:

        /**
         * @param node 
         */
        void append_node(Node* node)
        {
            if (!m_head) {
                ASSERT(!m_tail);
                m_head = node;
                m_tail = node;
                return;
            }

            ASSERT(m_tail);
            ASSERT(!node->next);
            m_tail->next = node;
            node->prev = m_tail;

            m_tail = node;
        }

        /**
         * @param node 
         */
        void prepend_node(Node* node)
        {
            if (!m_head) {
                ASSERT(!m_tail);
                m_head = node;
                m_tail = node;
                return;
            }

            ASSERT(m_tail);
            ASSERT(!node->prev);

            m_head->prev = node;
            node->next = m_head;
            m_head = node;
        }

        /**
         * @param value 
         * @return Node* 
         */
        Node* find_node(const T& value) const
        {
            for(auto* node = m_head; node; node = node->next) {
                if (Traits<T>::equals(node->value, value)) 
                    return node;
            }

            return nullptr;
        }

        /**
         * @return Node* 
         */
        Node* head()
        {
            return m_head;
        }

        /**
         * @return const Node* 
         */
        const Node* head() const
        {
            return m_head;
        }

        /**
         * @return Node* 
         */
        Node* tail() 
        {
            return m_tail;
        }

        /**
         * @return const Node* 
         */
        const Node* tail() const
        {
            return m_tail;
        }
    
        Node* m_head { nullptr };
        Node* m_tail { nullptr };
        
        
    }; // class DoubleLinkedList

} // namespace Mods 