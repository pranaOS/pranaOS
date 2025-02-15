/**
 * @file singlelinkedlist.h
 * @author Krisna Pranav
 * @brief Single Linked List
 * @version 6.0
 * @date 2023-07-25
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/find.h>
#include <mods/stdlibextra.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam ListType 
     * @tparam ElementType 
     */
    template <typename ListType, typename ElementType>
    class SinglyLinkedListIterator
    {
    public:
        /**
         * @brief Construct a new Singly Linked List Iterator object
         * 
         */
        SinglyLinkedListIterator() = default;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(SinglyLinkedListIterator const& other) const
        {
            return m_node != other.m_node;
        }

        /**
         * @return SinglyLinkedListIterator& 
         */
        SinglyLinkedListIterator& operator++()
        {
            if(m_removed)
                m_removed = false;
            else
                m_prev = m_node;
            m_node = m_next;
            if(m_next)
                m_next = m_next->next;
            return *this;
        }

        /**
         * @return ElementType& 
         */
        ElementType& operator*()
        {
            VERIFY(!m_removed);
            return m_node->value;
        }

        /**
         * @return ElementType* 
         */
        ElementType* operator->()
        {
            VERIFY(!m_removed);
            return &m_node->value;
        }

        bool is_end() const
        {
            return !m_node;
        }

        bool is_begin() const
        {
            return !m_prev;
        }

        /**
         * @param list 
         */
        void remove(ListType& list)
        {
            m_removed = true;
            list.remove(*this);
        };

    private:
        friend ListType;

        /**
         * @brief Construct a new Singly Linked List Iterator object
         * 
         * @param node 
         * @param prev 
         */
        explicit SinglyLinkedListIterator(typename ListType::Node* node, typename ListType::Node* prev = nullptr)
            : m_node(node), m_prev(prev), m_next(node ? node->next : nullptr)
        {
        }
        typename ListType::Node* m_node{nullptr};
        typename ListType::Node* m_prev{nullptr};
        typename ListType::Node* m_next{nullptr};
        bool m_removed{false};
    }; // class SinglyLinkedListIterator
    
    /**
     * @tparam T 
     */
    template <typename T>
    class SinglyLinkedList
    {
    private:
        struct Node
        {
            /**
             * @brief Construct a new Node object
             * 
             * @param v 
             */
            explicit Node(T&& v)
                : value(move(v))
            {
            }

            /**
             * @brief Construct a new Node object
             * 
             * @param v 
             */
            explicit Node(const T& v)
                : value(v)
            {
            }

            T value;
            Node* next{nullptr};
        }; // struct Node

    public:
        /**
         * @brief Construct a new Singly Linked List object
         * 
         */
        SinglyLinkedList() = default;

        /**
         * @brief Construct a new Singly Linked List object
         * 
         * @param other 
         */
        SinglyLinkedList(SinglyLinkedList const& other) = delete;

        /**
         * @brief Construct a new Singly Linked List object
         * 
         * @param other 
         */
        SinglyLinkedList(SinglyLinkedList&& other)
            : m_head(other.m_head), m_tail(other.m_tail)
        {
            other.m_head = nullptr;
            other.m_tail = nullptr;
        }

        /**
         * @param other 
         * @return SinglyLinkedList& 
         */
        SinglyLinkedList& operator=(SinglyLinkedList const& other) = delete;

        /**
         * @return SinglyLinkedList& 
         */
        SinglyLinkedList& operator=(SinglyLinkedList&&) = delete;

        /**
         * @brief Destroy the Singly Linked List object
         * 
         */
        ~SinglyLinkedList()
        {
            clear();
        }

        bool is_empty() const
        {
            return !head();
        }

        inline size_t size_slow() const
        {
            size_t size = 0;
            for(auto* node = m_head; node; node = node->next)
                ++size;
            return size;
        }

        void clear()
        {
            for(auto* node = m_head; node;)
            {
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
            VERIFY(head());
            return head()->value;
        }

        /**
         * @return const T& 
         */
        const T& first() const
        {
            VERIFY(head());
            return head()->value;
        }

        /**
         * @return T& 
         */
        T& last()
        {
            VERIFY(head());
            return tail()->value;
        }

        /**
         * @return const T& 
         */
        const T& last() const
        {
            VERIFY(head());
            return tail()->value;
        }

        /**
         * @return T 
         */
        T take_first()
        {
            VERIFY(m_head);
            auto* prev_head = m_head;
            T value = move(first());

            if(m_tail == m_head)
                m_tail = nullptr;

            m_head = m_head->next;
            delete prev_head;
            return value;
        }

        /**
         * @tparam U 
         * @param value 
         */
        template <typename U = T>
        void append(U&& value)
        {
            auto* node = new Node(forward<U>(value));
            if(!m_head)
            {
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
        ConstIterator end() const
        {
            return {};
        }

        /**
         * @tparam TUnaryPredicate 
         * @param pred 
         * @return ConstIterator 
         */
        template <typename TUnaryPredicate>
        ConstIterator find_if(TUnaryPredicate&& pred) const
        {
            return Mods::find_if(begin(), end(), forward<TUnaryPredicate>(pred));
        }

        /**
         * @tparam TUnaryPredicate 
         * @param pred 
         * @return Iterator 
         */
        template <typename TUnaryPredicate>
        Iterator find_if(TUnaryPredicate&& pred)
        {
            return Mods::find_if(begin(), end(), forward<TUnaryPredicate>(pred));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const
        {
            return find_if([&](auto& other)
                        { return Traits<T>::equals(value, other); });
        }   

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value)
        {
            return find_if([&](auto& other)
                        { return Traits<T>::equals(value, other); });
        }

        /**
         * @tparam U 
         * @param iterator 
         * @param value 
         */
        template <typename U = T>
        void insert_before(Iterator iterator, U&& value)
        {
            auto* node = new Node(forward<U>(value));
            node->next = iterator.m_node;
            if(m_head == iterator.m_node)
                m_head = node;
            if(iterator.m_prev)
                iterator.m_prev->next = node;
        }

        /**
         * @tparam U 
         * @param iterator 
         * @param value 
         */
        template <typename U = T>
        void insert_after(Iterator iterator, U&& value)
        {
            if(iterator.is_end())
            {
                append(value);
                return;
            }

            auto* node = new Node(forward<U>(value));
            node->next = iterator.m_node->next;

            iterator.m_node->next = node;

            if(m_tail == iterator.m_node)
                m_tail = node;
        }

        /**
         * @param iterator 
         */
        void remove(Iterator& iterator)
        {
            VERIFY(!iterator.is_end());

            if(m_head == iterator.m_node)
                m_head = iterator.m_node->next;
            if(m_tail == iterator.m_node)
                m_tail = iterator.m_prev;
            if(iterator.m_prev)
                iterator.m_prev->next = iterator.m_node->next;
            delete iterator.m_node;
        }

    private:
        Node* head()
        {
            return m_head;
        }

        Node const* head() const
        {
            return m_head;
        }

        Node* tail()
        {
            return m_tail;
        }

        Node const* tail() const
        {
            return m_tail;
        }

        Node* m_head{nullptr};
        Node* m_tail{nullptr};
    }; // class SinglyLinkedList
} // namespace Mods

using Mods::SinglyLinkedList;