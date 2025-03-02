/**
 * @file inlinelinkedlist.h
 * @author Krisna Pranav
 * @brief inlinelinkedlist
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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
        bool operator!=(const InlineLinkedListIterator& other) const 
        { 
            return m_node != other.m_node; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const InlineLinkedListIterator& other) const 
        { 
            return m_node == other.m_node; 
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

        /**
         * @param node 
         */
        explicit InlineLinkedListIterator(T* node)
            : m_node(node)
        {
        }
        T* m_node;
    };

    /**
     * @tparam T 
     */
    template<typename T>
    class InlineLinkedListNode 
    {
    public:
        InlineLinkedListNode();

        /**
         * @return * void 
         */
        void set_prev(T*);

        /**
         * @return * void 
         */
        void set_next(T*);

        /**
         * @return T* 
         */
        T* prev() const;
        T* next() const;
    };
    
    /**
     * @tparam T 
     */
    template<typename T>
    inline InlineLinkedListNode<T>::InlineLinkedListNode()
    {
        set_prev(0);
        set_next(0);
    }

    /**
     * @tparam T 
     * @param prev 
     */
    template<typename T>
    inline void InlineLinkedListNode<T>::set_prev(T* prev)
    {
        static_cast<T*>(this)->m_prev = prev;
    }

    /**
     * @tparam T 
     * @param next 
     */
    template<typename T>
    inline void InlineLinkedListNode<T>::set_next(T* next)
    {
        static_cast<T*>(this)->m_next = next;
    }

    /**
     * @tparam T 
     * @return T* 
     */
    template<typename T>
    inline T* InlineLinkedListNode<T>::prev() const
    {
        return static_cast<const T*>(this)->m_prev;
    }

    /**
     * @tparam T 
     * @return T* 
     */
    template<typename T>
    inline T* InlineLinkedListNode<T>::next() const
    {
        return static_cast<const T*>(this)->m_next;
    }

    template<typename T>
    class InlineLinkedList 
    {
    public:
        InlineLinkedList() { }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return !m_head; 
        }
        
        size_t size_slow() const;
        void clear();

        /**
         * @return T* 
         */
        T* head() const 
        { 
            return m_head; 
        }

        T* remove_head();
        T* remove_tail();

        /**
         * @return T* 
         */
        T* tail() const 
        { 
            return m_tail; 
        }

        void prepend(T*);
        void append(T*);
        void remove(T*);
        void append(InlineLinkedList<T>&);
        void insert_before(T*, T*);
        void insert_after(T*, T*);
        
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(T* value) const
        {
            for (T* node = m_head; node; node = node->next()) {
                if (node == value)
                    return true;
            }
            return false;
        }

        /**
         * @tparam F 
         * @param func 
         * @return IterDecision 
         */
        template<typename F>
        IterDecision for_each(F func) const
        {
            for (T* node = m_head; node; node = node->next()) {
                IterDecision decision = func(*node);
                if (decision != IterDecision::Continue)
                    return decision;
            }
            return IterDecision::Continue;
        }

        using Iterator = InlineLinkedListIterator<T>;
        friend Iterator;

        /**
         * @brief begin
         * 
         * @return Iterator 
         */
        Iterator begin() 
        { 
            return Iterator(m_head); 
        }

        /**
         * @brief end
         * 
         * @return Iterator 
         */
        Iterator end() 
        { 
            return Iterator::universal_end(); 
        }

        using ConstIterator = InlineLinkedListIterator<const T>;
        friend ConstIterator;

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const 
        { 
            return ConstIterator(m_head); 
        }

        /**
         * @brief end
         * 
         * @return ConstIterator 
         */
        ConstIterator end() const 
        { 
            return ConstIterator::universal_end(); 
        }

    private:
        T* m_head { nullptr };
        T* m_tail { nullptr };
    };

    /**
     * @tparam T 
     * @return size_t 
     */
    template<typename T>
    inline size_t InlineLinkedList<T>::size_slow() const
    {
        size_t size = 0;
        for (T* node = m_head; node; node = node->next())
            ++size;
        return size;
    }

    /**
     * @tparam T 
     */
    template<typename T>
    inline void InlineLinkedList<T>::clear()
    {
        m_head = 0;
        m_tail = 0;
    }
    
    /**
     * @tparam T 
     * @param node 
     */
    template<typename T>
    inline void InlineLinkedList<T>::prepend(T* node)
    {
        if (!m_head) {
            ASSERT(!m_tail);
            m_head = node;
            m_tail = node;
            node->set_prev(0);
            node->set_next(0);
            return;
        }

        ASSERT(m_tail);
        m_head->set_prev(node);
        node->set_next(m_head);
        node->set_prev(0);
        m_head = node;
    }

    /**
     * @tparam T 
     * @param node 
     */
    template<typename T>
    inline void InlineLinkedList<T>::append(T* node)
    {
        if (!m_tail) {
            ASSERT(!m_head);
            m_head = node;
            m_tail = node;
            node->set_prev(0);
            node->set_next(0);
            return;
        }

        ASSERT(m_head);
        m_tail->set_next(node);
        node->set_prev(m_tail);
        node->set_next(0);
        m_tail = node;
    }

    /**
     * @tparam T 
     * @param before_node 
     * @param node 
     */
    template<typename T>
    inline void InlineLinkedList<T>::insert_before(T* before_node, T* node)
    {
        ASSERT(before_node);
        ASSERT(node);
        ASSERT(before_node != node);
        ASSERT(!is_empty());
        if (m_head == before_node) {
            ASSERT(!before_node->prev());
            m_head = node;
            node->set_prev(0);
            node->set_next(before_node);
            before_node->set_prev(node);
        } else {
            ASSERT(before_node->prev());
            node->set_prev(before_node->prev());
            before_node->prev()->set_next(node);
            node->set_next(before_node);
            before_node->set_prev(node);
        }
    }

    /**
     * @tparam T 
     * @param after_node 
     * @param node 
     */
    template<typename T>
    inline void InlineLinkedList<T>::insert_after(T* after_node, T* node)
    {
        ASSERT(after_node);
        ASSERT(node);
        ASSERT(after_node != node);
        ASSERT(!is_empty());
        if (m_tail == after_node) {
            ASSERT(!after_node->next());
            m_tail = node;
            node->set_prev(after_node);
            node->set_next(0);
            after_node->set_next(node);
        } else {
            ASSERT(after_node->next());
            node->set_prev(after_node);
            node->set_next(after_node->next());
            after_node->next()->set_prev(node);
            after_node->set_next(node);
        }
    }

    /**
     * @tparam T 
     * @param node 
     */
    template<typename T>
    inline void InlineLinkedList<T>::remove(T* node)
    {
        if (node->prev()) {
            ASSERT(node != m_head);
            node->prev()->set_next(node->next());
        } else {
            ASSERT(node == m_head);
            m_head = node->next();
        }

        if (node->next()) {
            ASSERT(node != m_tail);
            node->next()->set_prev(node->prev());
        } else {
            ASSERT(node == m_tail);
            m_tail = node->prev();
        }

        node->set_next(0);
        node->set_prev(0);
    }

    /**
     * @tparam T 
     * @return T* 
     */
    template<typename T>
    inline T* InlineLinkedList<T>::remove_head()
    {
        T* node = head();
        if (node)
            remove(node);
        return node;
    }

    /**
     * @return * template<typename T>* 
     */
    template<typename T>
    inline T* InlineLinkedList<T>::remove_tail()
    {
        T* node = tail();
        if (node)
            remove(node);
        return node;
    }

    /**
     * @tparam T 
     * @param other 
     */
    template<typename T>
    inline void InlineLinkedList<T>::append(InlineLinkedList<T>& other)
    {
        if (!other.head())
            return;

        if (!head()) {
            m_head = other.head();
            m_tail = other.tail();
            other.clear();
            return;
        }

        ASSERT(tail());
        ASSERT(other.head());
        T* other_head = other.head();
        T* other_tail = other.tail();
        other.clear();

        ASSERT(!m_tail->next());
        m_tail->set_next(other_head);
        ASSERT(!other_head->prev());
        other_head->set_prev(m_tail);
        m_tail = other_tail;
    }

}

using Mods::InlineLinkedList;
using Mods::InlineLinkedListNode;
