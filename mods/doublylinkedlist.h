/**
 * @file doublylinkedlist.h
 * @author Krisna Pranav
 * @brief Doubly Linked List
 * @version 6.0
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/find.h>
#include <mods/stdlibextra.h>

namespace Mods
{
    /**
     * @tparam ListType 
     * @tparam ElementType 
     */
    template<typename ListType, typename ElementType>
    class DoublyLinkedListIterator 
    {
    public:
        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(DoublyLinkedListIterator const& other) const 
        { 
            return m_node != other.m_node; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(DoublyLinkedListIterator const& other) const 
        { 
            return m_node == other.m_node; 
        }

        /**
         * @return DoublyLinkedListIterator& 
         */
        DoublyLinkedListIterator& operator++()
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
        [[nodiscard]] bool is_end() const 
        { 
            return !m_node; 
        }

        /**
         * @return DoublyLinkedListIterator 
         */
        static DoublyLinkedListIterator universal_end() 
        { 
            return DoublyLinkedListIterator(nullptr); 
        }

    private:
        friend ListType;

        /**
         * @brief Construct a new Doubly Linked List Iterator object
         * 
         * @param node 
         */
        explicit DoublyLinkedListIterator(typename ListType::Node* node)
            : m_node(node)
        {
        }

        typename ListType::Node* m_node;
    }; // class DoublyLinkedListIterator

    /**
     * @tparam T 
     */
    template<typename T>
    class DoublyLinkedList 
    {
    private:
        struct Node 
        {
            /**
             * @brief Construct a new Node object
             * 
             * @tparam U 
             * @param v 
             */
            template<typename U>
            explicit Node(U&& v)
                : value(forward<U>(v))
            {
                static_assert(
                    requires { T(v); }, "Conversion operator is missing.");
            }

            T value;
            Node* next { nullptr };
            Node* prev { nullptr };
        };

    public:
        /**
         * @brief Construct a new Doubly Linked List object
         * 
         */
        DoublyLinkedList() = default;

        /**
         * @brief Destroy the Doubly Linked List object
         * 
         */
        ~DoublyLinkedList() { clear(); }

        [[nodiscard]] bool is_empty() const 
        { 
            return !m_head; 
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

        [[nodiscard]] T& first()
        {
            VERIFY(m_head);
            return m_head->value;
        }
        
        [[nodiscard]] const T& first() const
        {
            VERIFY(m_head);
            return m_head->value;
        }

        [[nodiscard]] T& last()
        {
            VERIFY(m_head);
            return m_tail->value;
        }
        
        [[nodiscard]] const T& last() const
        {
            VERIFY(m_head);
            return m_tail->value;
        }

        /**
         * @tparam U 
         * @param value 
         */
        template<typename U>
        void append(U&& value)
        {
            static_assert(
                requires { T(value); }, "Conversion operator is missing.");

            auto* node = new Node(forward<U>(value));

            if (!m_head) {
                VERIFY(!m_tail);
                m_head = node;
                m_tail = node;
                return;
            }

            VERIFY(m_tail);
            VERIFY(!node->next);
            m_tail->next = node;
            node->prev = m_tail;
            m_tail = node;
        }

        /**
         * @tparam U 
         * @param value 
         */
        template<typename U>
        void prepend(U&& value)
        {
            static_assert(IsSame<T, U>);
            auto* node = new Node(forward<U>(value));

            if (!m_head) {
                VERIFY(!m_tail);
                m_head = node;
                m_tail = node;
                return;
            }

            VERIFY(m_tail);
            VERIFY(!node->prev);
            m_head->prev = node;
            node->next = m_head;
            m_head = node;
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool contains_slow(const T& value) const
        {
            return find(value) != end();
        }

        using Iterator = DoublyLinkedListIterator<DoublyLinkedList, T>;
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

        using ConstIterator = DoublyLinkedListIterator<const DoublyLinkedList, const T>;
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
            return Mods::find(begin(), end(), value);
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value)
        {
            return Mods::find(begin(), end(), value);
        }

        /**
         * @param it 
         */
        void remove(Iterator it)
        {
            VERIFY(it.m_node);
            auto* node = it.m_node;

            if (node->prev) {
                VERIFY(node != m_head);
                node->prev->next = node->next;
            } else {
                VERIFY(node == m_head);
                m_head = node->next;
            }

            if (node->next) {
                VERIFY(node != m_tail);
                node->next->prev = node->prev;
            } else {
                VERIFY(node == m_tail);
                m_tail = node->prev;
            }

            delete node;
        }

    private:
        Node* m_head { nullptr };
        Node* m_tail { nullptr };
    }; // class DoublyLinkedList
} // namespace Mods

using Mods::DoublyLinkedList;
