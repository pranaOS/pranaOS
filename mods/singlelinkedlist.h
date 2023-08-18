/**
 * @file singlelinkedlist.h
 * @author Krisna Pranav
 * @brief Single Linked List
 * @version 6.0
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

    template<typename ListType, typename ElementType>
    class SinglyLinkedListIterator 
    {
    public:
        SinglyLinkedListIterator() { }

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
        explicit SinglyLinkedListIterator(typename ListType::Node* node, typename ListType::Node* prev = nullptr)
            : m_node(node)
            , m_prev(prev)
        {}

        /// @brief m_node + m_prev
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

            /**
             * @param v 
             */
            explicit Node(const T& v)
                : value(v)
            {}

            T value;
            Node* next { nullptr };
        };

    public:
        SinglyLinkedList() { }
        ~SinglyLinkedList() { clear(); }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return !head(); 
        }

        /**
         * @return size_t 
         */
        inline size_t size_slow() const
        {
            size_t size = 0;
            for (auto* node = m_head; node; node = node->next)
                ++size;
            return size;
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
         * @return T 
         */
        T take_first()
        {
            ASSERT(m_head);
            auto* prev_head = m_head;
            T value = move(first());
            if (m_tail == m_head)
                m_tail = nullptr;
            m_head = m_head->next;
            delete prev_head;
            return value;
        }

        /**
         * @param value 
         */
        void append(const T& value)
        {
            append(T(value));
        }

        /**
         * @param value 
         */
        void append(T&& value)
        {
            auto* node = new Node(move(value));
            if (!m_head) {
                m_head = node;
                m_tail = node;
                return;
            }
            m_tail->next = node;
            m_tail = node;
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(const T& value) const
        {
            return find(value) != end();
        }

        using Iterator = SinglyLinkedListIterator<SinglyLinkedList, T>;
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
            return {}; 
        }

        using ConstIterator = SinglyLinkedListIterator<const SinglyLinkedList, const T>;
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
            return {}; 
        }

        /**
         * @tparam Finder 
         * @param finder 
         * @return ConstIterator 
         */
        template<typename Finder>
        ConstIterator find(Finder finder) const
        {
            Node* prev = nullptr;
            for (auto* node = m_head; node; node = node->next) {
                if (finder(node->value))
                    return ConstIterator(node, prev);
                prev = node;
            }
            return end();
        }

        /**
         * @tparam Finder 
         * @param finder 
         * @return Iterator 
         */
        template<typename Finder>
        Iterator find(Finder finder)
        {
            Node* prev = nullptr;
            for (auto* node = m_head; node; node = node->next) {
                if (finder(node->value))
                    return Iterator(node, prev);
                prev = node;
            }
            return end();
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const
        {
            return find([&](auto& other) { return Traits<T>::equals(value, other); });
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value)
        {
            return find([&](auto& other) { return Traits<T>::equals(value, other); });
        }

        /**
         * @param iterator 
         */
        void remove(Iterator iterator)
        {
            ASSERT(!iterator.is_end());
            if (m_head == iterator.m_node)
                m_head = iterator.m_node->next;
            if (m_tail == iterator.m_node)
                m_tail = iterator.m_prev;
            if (iterator.m_prev)
                iterator.m_prev->next = iterator.m_node->next;
            delete iterator.m_node;
        }

        /**
         * @param iterator 
         * @param value 
         */
        void insert_before(Iterator iterator, const T& value)
        {
            insert_before(iterator, T(value));
        }

        /**
         * @param iterator 
         * @param value 
         */
        void insert_before(Iterator iterator, T&& value)
        {
            auto* node = new Node(move(value));
            node->next = iterator.m_node;
            if (m_head == iterator.m_node)
                m_head = node;
            if (iterator.m_prev)
                iterator.m_prev->next = node;
        }

        /**
         * @param iterator 
         * @param value 
         */
        void insert_after(Iterator iterator, const T& value)
        {
            insert_after(iterator, T(value));
        }

        /**
         * @param iterator 
         * @param value 
         */
        void insert_after(Iterator iterator, T&& value)
        {
            if (iterator.is_end()) {
                append(value);
                return;
            }

            auto* node = new Node(move(value));
            node->next = iterator.m_node->next;

            iterator.m_node->next = node;

            if (m_tail == iterator.m_node)
                m_tail = node;
        }

    private:

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
    }; // class SinglyLinkedList

} // namespace Mods

using Mods::SinglyLinkedList;
