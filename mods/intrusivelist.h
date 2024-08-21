/**
 * @file intrusivelist.h
 * @author Krisna Pranav
 * @brief IntrusiveList
 * @version 6.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "assertions.h"

namespace Mods 
{

    class IntrusiveListNode;

    class IntrusiveListStorage 
    {
    private:
        friend class IntrusiveListNode;

        template<class T, IntrusiveListNode T::*member>
        friend class IntrusiveList;

        /// @brief: m_first, m_last
        IntrusiveListNode* m_first { nullptr };
        IntrusiveListNode* m_last { nullptr };
    }; // class IntrusiveListStorage

    template<class T, IntrusiveListNode T::*member>
    class IntrusiveList 
    {
    public:
        IntrusiveList();
        ~IntrusiveList();

        /// @brief: clear
        void clear();

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const;

        /**
         * @brief append
         * 
         * @param n 
         */
        void append(T& n);

        /**
         * @param n 
         */
        void prepend(T& n);

        /**
         * @param n 
         */
        void remove(T& n);

        /**
         * @return true 
         * @return false 
         */
        bool contains(const T&) const;

        /**
         * @return T* 
         */
        T* first() const;
        T* last() const;
        T* take_first();
        T* take_last();

        class Iterator 
        {
        public:
            Iterator();
            Iterator(T* value);

            /**
             * @return T& 
             */
            T& operator*() const;

            /**
             * @return T* 
             */
            T* operator->() const;

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(const Iterator& other) const;

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(const Iterator& other) const 
            { 
                return !(*this == other); 
            }

            /**
             * @return Iterator& 
             */
            Iterator& operator++();

            /**
             * @return Iterator& 
             */
            Iterator& erase();

        private:
            /// @brief: m_value
            T* m_value { nullptr };
        };

        /// @brief: begin, end
        Iterator begin();
        Iterator end();

    private:
        /**
         * @param current 
         * @return T* 
         */
        static T* next(T* current);

        /**
         * @param node 
         * @return T* 
         */
        static T* node_to_value(IntrusiveListNode& node);

        /// @brief m_storage
        IntrusiveListStorage m_storage;
    }; // class IntrusiveList

    class IntrusiveListNode 
    {
    public:
        ~IntrusiveListNode();

        void remove();

        /**
         * @return true 
         * @return false 
         */
        bool is_in_list() const;

    private:
        template<class T, IntrusiveListNode T::*member>
        friend class IntrusiveList;
        IntrusiveListStorage* m_storage = nullptr;
        IntrusiveListNode* m_next = nullptr;
        IntrusiveListNode* m_prev = nullptr;
    }; // class IntrusiveListNode

    /**
     * @tparam T 
     * @tparam T::*member 
     */
    template<class T, IntrusiveListNode T::*member>
    inline IntrusiveList<T, member>::Iterator::Iterator()
    {
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param value 
     */
    template<class T, IntrusiveListNode T::*member>
    inline IntrusiveList<T, member>::Iterator::Iterator(T* value)
        : m_value(value)
    {}

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T& 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T& IntrusiveList<T, member>::Iterator::operator*() const
    {
        return *m_value;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::Iterator::operator->() const
    {
        return m_value;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param other 
     * @return true 
     * @return false 
     */
    template<class T, IntrusiveListNode T::*member>
    inline bool IntrusiveList<T, member>::Iterator::operator==(const Iterator& other) const
    {
        return other.m_value == m_value;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return IntrusiveList<T, member>::Iterator& 
     */
    template<class T, IntrusiveListNode T::*member>
    inline typename IntrusiveList<T, member>::Iterator& IntrusiveList<T, member>::Iterator::operator++()
    {
        m_value = IntrusiveList<T, member>::next(m_value);
        return *this;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return IntrusiveList<T, member>::Iterator& 
     */
    template<class T, IntrusiveListNode T::*member>
    inline typename IntrusiveList<T, member>::Iterator& IntrusiveList<T, member>::Iterator::erase()
    {
        T* old = m_value;
        m_value = IntrusiveList<T, member>::next(m_value);
        (old->*member).remove();
        return *this;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     */
    template<class T, IntrusiveListNode T::*member>
    inline IntrusiveList<T, member>::IntrusiveList()

    {}

    /**
     * @tparam T 
     * @tparam T::*member 
     */
    template<class T, IntrusiveListNode T::*member>
    inline IntrusiveList<T, member>::~IntrusiveList()
    {
        clear();
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     */
    template<class T, IntrusiveListNode T::*member>
    inline void IntrusiveList<T, member>::clear()
    {
        while (m_storage.m_first)
            m_storage.m_first->remove();
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return true 
     * @return false 
     */
    template<class T, IntrusiveListNode T::*member>
    inline bool IntrusiveList<T, member>::is_empty() const
    {
        return m_storage.m_first == nullptr;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param n 
     */
    template<class T, IntrusiveListNode T::*member>
    inline void IntrusiveList<T, member>::append(T& n)
    {
        auto& nnode = n.*member;
        if (nnode.m_storage)
            nnode.remove();

        nnode.m_storage = &m_storage;
        nnode.m_prev = m_storage.m_last;
        nnode.m_next = nullptr;

        if (m_storage.m_last)
            m_storage.m_last->m_next = &nnode;
        m_storage.m_last = &nnode;
        if (!m_storage.m_first)
            m_storage.m_first = &nnode;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param n 
     */
    template<class T, IntrusiveListNode T::*member>
    inline void IntrusiveList<T, member>::prepend(T& n)
    {
        auto& nnode = n.*member;
        if (nnode.m_storage)
            nnode.remove();

        nnode.m_storage = &m_storage;
        nnode.m_prev = nullptr;
        nnode.m_next = m_storage.m_first;

        if (m_storage.m_first)
            m_storage.m_first->m_prev = &nnode;
        m_storage.m_first = &nnode;
        if (!m_storage.m_last)
            m_storage.m_last = &nnode;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param n 
     */
    template<class T, IntrusiveListNode T::*member>
    inline void IntrusiveList<T, member>::remove(T& n)
    {
        auto& nnode = n.*member;
        if (nnode.m_storage)
            nnode.remove();
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param n 
     * @return true 
     * @return false 
     */
    template<class T, IntrusiveListNode T::*member>
    inline bool IntrusiveList<T, member>::contains(const T& n) const
    {
        auto& nnode = n.*member;
        return nnode.m_storage == &m_storage;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::first() const
    {
        return m_storage.m_first ? node_to_value(*m_storage.m_first) : nullptr;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::take_first()
    {
        if (auto* ptr = first()) {
            remove(*ptr);
            return ptr;
        }
        return nullptr;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::take_last()
    {
        if (auto* ptr = last()) {
            remove(*ptr);
            return ptr;
        }
        return nullptr;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::last() const
    {
        return m_storage.m_last ? node_to_value(*m_storage.m_last) : nullptr;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param current 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::next(T* current)
    {
        auto& nextnode = (current->*member).m_next;
        T* nextstruct = nextnode ? node_to_value(*nextnode) : nullptr;
        return nextstruct;
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return IntrusiveList<T, member>::Iterator 
     */
    template<class T, IntrusiveListNode T::*member>
    inline typename IntrusiveList<T, member>::Iterator IntrusiveList<T, member>::begin()
    {
        return m_storage.m_first ? Iterator(node_to_value(*m_storage.m_first)) : Iterator();
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @return IntrusiveList<T, member>::Iterator 
     */
    template<class T, IntrusiveListNode T::*member>
    inline typename IntrusiveList<T, member>::Iterator IntrusiveList<T, member>::end()
    {
        return Iterator();
    }

    /**
     * @tparam T 
     * @tparam T::*member 
     * @param node 
     * @return T* 
     */
    template<class T, IntrusiveListNode T::*member>
    inline T* IntrusiveList<T, member>::node_to_value(IntrusiveListNode& node)
    {
        return (T*)((char*)&node - ((char*)&(((T*)nullptr)->*member) - (char*)nullptr));
    }

    /// @brief: IntrusiveListNode
    inline IntrusiveListNode::~IntrusiveListNode()
    {
        if (m_storage)
            remove();
    }

    /// @brief: remove
    inline void IntrusiveListNode::remove()
    {
        ASSERT(m_storage);
        if (m_storage->m_first == this)
            m_storage->m_first = m_next;
        if (m_storage->m_last == this)
            m_storage->m_last = m_prev;
        if (m_prev)
            m_prev->m_next = m_next;
        if (m_next)
            m_next->m_prev = m_prev;
        m_prev = nullptr;
        m_next = nullptr;
        m_storage = nullptr;
    }

    /**
     * @return true 
     * @return false 
     */
    inline bool IntrusiveListNode::is_in_list() const
    {
        return m_storage != nullptr;
    }

} // namespace Mods

using Mods::IntrusiveList;
using Mods::IntrusiveListNode;
