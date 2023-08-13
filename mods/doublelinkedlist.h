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

    /**
     * @tparam ListType 
     * @tparam ElementType 
     */
    template<typename ListType, typename ElementType>
    class DoubleLinkedListIterator 
    {
    public:

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const DoubleLinkedListIterator& other) const 
        { 
            return m_node != other.m_node; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
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

        /**
         * @return true 
         * @return false 
         */
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

        /**
         * @param node 
         */
        explicit DoubleLinkedListIterator(typename ListType::Node* node)
            : m_node(node)
        { }

        typename ListType::Node* m_node;

    }; // class DoubleLinkedListIterator

    template<typename T>
    class DoubleLinkedList 
    {
    private:
        struct Node 
        {
            /**
             * @param v 
             */
            explicit Node(const T& v)
                : value(v)
            {
            }

            /**
             * @brief Construct a new Node object
             * 
             * @param v 
             */
            explicit Node(T&& v)
                : value(move(v))
            {
            }

            T value;
            Node* next { nullptr };
            Node* prev { nullptr };
        }; // struct Node

    public:
        DoubleLinkedList() { }
        ~DoubleLinkedList() { clear(); }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return !head(); 
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

        /**
         * @return T& 
         */
        T& first()
        {
            ASSERT(head());
            return head()->value;
        }

        /**
         * @return const T& 
         */
        const T& first() const
        {
            ASSERT(head());
            return head()->value;
        }

        /**
         * @return T& 
         */
        T& last()
        {
            ASSERT(head());
            return tail()->value;
        }

        /**
         * @return const T& 
         */
        const T& last() const
        {
            ASSERT(head());
            return tail()->value;
        }

        /**
         * @param value 
         */
        void append(T&& value)
        {
            append_node(new Node(move(value)));
        }

        /**
         * @param value 
         */
        void append(const T& value)
        {
            append_node(new Node(value));
        }

        /**
         * @param value 
         */
        void prepend(T&& value)
        {
            prepend_node(new Node(move(value)));
        }

        /**
         * @param value 
         */
        void prepend(const T& value)
        {
            prepend_node(new Node(value));
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(const T& value) const
        {
            return find_node(value) != nullptr;
        }

        using Iterator = DoubleLinkedListIterator<DoubleLinkedList, T>;
        friend Iterator;

        /**
         * @return Iterator 
         */
        Iterator begin() 
        { 
            return Iterator(m_head); 
        }

        /**
         * @return Iterator 
         */
        Iterator end() 
        { 
            return Iterator::universal_end(); 
        }

        using ConstIterator = DoubleLinkedListIterator<const DoubleLinkedList, const T>;
        friend ConstIterator;

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const 
        { 
            return ConstIterator(m_head); 
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const 
        { 
            return ConstIterator::universal_end(); 
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const
        {
            Node* node = find_node(value);

            if (node)
                return ConstIterator(node);

            return end();
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value)
        {
            Node* node = find_node(value);

            if (node)
                return Iterator(node);

            return end();
        }

        /**
         * @param it 
         */
        void remove(Iterator it)
        {
            ASSERT(it.m_node);

            auto* node = it.m_node;

            if (node->prev) {
                ASSERT(node != m_head);
                node->prev->next = node->next;
            } else {
                ASSERT(node == m_head);
                m_head = node->next;
            }
            if (node->next) {
                ASSERT(node != m_tail);
                node->next->prev = node->prev;
            } else {
                ASSERT(node == m_tail);
                m_tail = node->prev;
            }

            delete node;
        }

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
            for (auto* node = m_head; node; node = node->next) {
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

        /// @brief: m_head + m_tail
        Node* m_head { nullptr };
        Node* m_tail { nullptr };
    }; // class DoubleLinkedList

}

using Mods::DoubleLinkedList;
