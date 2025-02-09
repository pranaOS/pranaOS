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

#include <mods/assertions.h>
#include <mods/bitcast.h>
#include <mods/forward.h>
#include <mods/intrusivedetails.h>
#include <mods/noncopyable.h>
#include <mods/stdlibextra.h>

namespace Mods::Detail
{
    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container = RawPtr<T>>
    class IntrusiveListNode;

    struct ExtractIntrusiveListTypes
    {
        /**
         * @tparam V 
         * @tparam Container 
         * @tparam T 
         * @param x 
         * @return V 
         */
        template <typename V, typename Container, typename T>
        static V value(IntrusiveListNode<V, Container> T::* x);

        /**
         * @tparam V 
         * @tparam Container 
         * @tparam T 
         * @param x 
         * @return Container 
         */
        template <typename V, typename Container, typename T>
        static Container container(IntrusiveListNode<V, Container> T::* x);
    }; // struct ExtractIntrusiveListTypes

    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container = RawPtr<T>>
    using SubstitutedIntrusiveListNode = IntrusiveListNode<T, typename Detail::SubstituteIntrusiveContainerType<T, Container>::Type>;

    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container>
    class IntrusiveListStorage
    {
    private:
        friend class IntrusiveListNode<T, Container>;

        /**
         * @tparam T_ 
         * @tparam Container_ 
         * @tparam member 
         */
        template <class T_, typename Container_, SubstitutedIntrusiveListNode<T_, Container_> T_::* member>
        friend class IntrusiveList;

        SubstitutedIntrusiveListNode<T, Container>* m_first{nullptr};
        SubstitutedIntrusiveListNode<T, Container>* m_last{nullptr};
    }; // class IntrusiveListStorage

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    class IntrusiveList
    {
        MOD_MAKE_NONCOPYABLE(IntrusiveList);
        MOD_MAKE_NONMOVABLE(IntrusiveList);

    public:
        /**
         * @brief Construct a new Intrusive List object
         * 
         */
        IntrusiveList() = default;

        /**
         * @brief Destroy the Intrusive List object
         * 
         */
        ~IntrusiveList();

        void clear();

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const;

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t size_slow() const;

        /**
         * @param n 
         */
        void append(T& n);

        /**
         * @param n 
         */
        void prepend(T& n);

        void insert_before(T&, T&);

        /**
         * @param n 
         */
        void remove(T& n);

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool contains(const T&) const;

        /**
         * @return Container 
         */
        [[nodiscard]] Container first() const;
        [[nodiscard]] Container last() const;
        [[nodiscard]] Container take_first();
        [[nodiscard]] Container take_last();

        class Iterator
        {
        public:
            /**
             * @brief Construct a new Iterator object
             * 
             */
            Iterator() = default;

            /**
             * @brief Construct a new Iterator object
             * 
             * @param value 
             */
            Iterator(T* value)
                : m_value(move(value))
            {
            }

            /**
             * @return const T& 
             */
            const T& operator*() const
            {
                return *m_value;
            }

            /**
             * @return auto 
             */
            auto operator->() const
            {
                return m_value;
            }

            /**
             * @return T& 
             */
            T& operator*()
            {
                return *m_value;
            }

            /**
             * @return auto 
             */
            auto operator->()
            {
                return m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(Iterator const& other) const
            {
                return other.m_value == m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(Iterator const& other) const
            {
                return !(*this == other);
            }

            /**
             * @return Iterator& 
             */
            Iterator& operator++()
            {
                m_value = IntrusiveList<T, Container, member>::next(m_value);
                return *this;
            }
            
            Iterator& erase();

        private:
            T* m_value{nullptr};
        }; // class Iterator

        Iterator begin();

        Iterator end()
        {
            return Iterator{};
        }

        class ReverseIterator
        {
        public:
            /**
             * @brief Construct a new Reverse Iterator object
             * 
             */
            ReverseIterator() = default;

            /**
             * @brief Construct a new Reverse Iterator object
             * 
             * @param value 
             */
            ReverseIterator(T* value)
                : m_value(move(value))
            {
            }

            /**
             * @return const T& 
             */
            const T& operator*() const
            {
                return *m_value;
            }

            /**
             * @return auto 
             */
            auto operator->() const
            {
                return m_value;
            }
        
            /**
             * @return T& 
             */
            T& operator*()
            {
                return *m_value;
            }

            /**
             * @return auto 
             */
            auto operator->()
            {
                return m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(ReverseIterator const& other) const
            {
                return other.m_value == m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(ReverseIterator const& other) const
            {
                return !(*this == other);
            }

            /**
             * @return ReverseIterator& 
             */
            ReverseIterator& operator++()
            {
                m_value = IntrusiveList<T, Container, member>::prev(m_value);
                return *this;
            }

            /**
             * @return ReverseIterator& 
             */
            ReverseIterator& erase();

        private:
            T* m_value{nullptr};
        }; // class ReverseIterator

        ReverseIterator rbegin();

        ReverseIterator rend()
        {
            return ReverseIterator{};
        }

        class ConstIterator
        {
        public:
            /**
             * @brief Construct a new Const Iterator object
             * 
             */
            ConstIterator() = default;

            /**
             * @brief Construct a new Const Iterator object
             * 
             * @param value 
             */
            ConstIterator(const T* value)
                : m_value(value)
            {
            }

            /**
             * @return const T& 
             */
            const T& operator*() const
            {
                return *m_value;
            }

            /**
             * @return auto 
             */
            auto operator->() const
            {
                return m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(ConstIterator const& other) const
            {
                return other.m_value == m_value;
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(ConstIterator const& other) const
            {
                return !(*this == other);
            }

            /**
             * @return ConstIterator& 
             */
            ConstIterator& operator++()
            {
                m_value = IntrusiveList<T, Container, member>::next(m_value);
                return *this;
            }

        private:
            const T* m_value{nullptr};
        }; // class ConstIterator

        ConstIterator begin() const;
        ConstIterator end() const
        {
            return ConstIterator{};
        }

    private:    
        /**
         * @param current 
         * @return T* 
         */
        static T* next(T* current);

        /**
         * @param current 
         * @return T* 
         */
        static T* prev(T* current);

        /**
         * @param current 
         * @return const T* 
         */
        static const T* next(const T* current);

        /**
         * @param current 
         * @return const T* 
         */
        static const T* prev(const T* current);

        /**
         * @param node 
         * @return T* 
         */
        static T* node_to_value(SubstitutedIntrusiveListNode<T, Container>& node);
        IntrusiveListStorage<T, Container> m_storage;
    }; // class IntrusiveList

    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container>
    class IntrusiveListNode
    {
    public:
        /**
         * @brief Destroy the Intrusive List Node object
         * 
         */
        ~IntrusiveListNode();

        void remove();
        bool is_in_list() const;

        static constexpr bool IsRaw = IsPointer<Container>;

    #ifndef __clang__
    private:
        template <class T_, typename Container_, SubstitutedIntrusiveListNode<T_, Container_> T_::* member>
        friend class ::Mods::Detail::IntrusiveList;
    #endif

        IntrusiveListStorage<T, Container>* m_storage = nullptr;
        SubstitutedIntrusiveListNode<T, Container>* m_next = nullptr;
        SubstitutedIntrusiveListNode<T, Container>* m_prev = nullptr;
        [[no_unique_address]] SelfReferenceIfNeeded<Container, IsRaw> m_self;
    }; // class IntrusiveListNode

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return IntrusiveList<T, Container, member>::Iterator& 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline typename IntrusiveList<T, Container, member>::Iterator& IntrusiveList<T, Container, member>::Iterator::erase()
    {
        auto old = m_value;
        m_value = IntrusiveList<T, Container, member>::next(m_value);
        (old->*member).remove();
        return *this;
    }
    
    /**
     * @brief Destroy the Intrusive List< T,  Container, member>:: Intrusive List object
     * 
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline IntrusiveList<T, Container, member>::~IntrusiveList()
    {
        clear();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline void IntrusiveList<T, Container, member>::clear()
    {
        while(m_storage.m_first)
            m_storage.m_first->remove();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return true 
     * @return false 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline bool IntrusiveList<T, Container, member>::is_empty() const
    {
        return m_storage.m_first == nullptr;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return size_t 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline size_t IntrusiveList<T, Container, member>::size_slow() const
    {
        size_t size = 0;
        auto it_end = end();
        for(auto it = begin(); it != it_end; ++it)
        {
            ++size;
        }
        return size;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param n 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline void IntrusiveList<T, Container, member>::append(T& n)
    {
        remove(n);

        auto& nnode = n.*member;
        nnode.m_storage = &m_storage;
        nnode.m_prev = m_storage.m_last;
        nnode.m_next = nullptr;

        if constexpr(!RemoveReference<decltype(nnode)>::IsRaw)
            nnode.m_self.reference = &n; 

        if(m_storage.m_last)
            m_storage.m_last->m_next = &nnode;

        m_storage.m_last = &nnode;

        if(!m_storage.m_first)
            m_storage.m_first = &nnode;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param n 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline void IntrusiveList<T, Container, member>::prepend(T& n)
    {
        remove(n);

        auto& nnode = n.*member;
        
        nnode.m_storage = &m_storage;
        nnode.m_prev = nullptr;
        nnode.m_next = m_storage.m_first;

        if constexpr(!RemoveReference<decltype(nnode)>::IsRaw)
            nnode.m_self.reference = &n;

        if(m_storage.m_first)
            m_storage.m_first->m_prev = &nnode;

        m_storage.m_first = &nnode;

        if(!m_storage.m_last)
            m_storage.m_last = &nnode;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param bn 
     * @param n 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline void IntrusiveList<T, Container, member>::insert_before(T& bn, T& n)
    {
        remove(n);

        auto& new_node = n.*member;
        auto& before_node = bn.*member;

        new_node.m_storage = &m_storage;
        new_node.m_next = &before_node;
        new_node.m_prev = before_node.m_prev;

        if(before_node.m_prev)
            before_node.m_prev->m_next = &new_node;

        before_node.m_prev = &new_node;

        if(m_storage.m_first == &before_node)
        {
            m_storage.m_first = &new_node;
        }

        if constexpr(!RemoveReference<decltype(new_node)>::IsRaw)
            new_node.m_self.reference = &n;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param n 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline void IntrusiveList<T, Container, member>::remove(T& n)
    {
        auto& nnode = n.*member;
        if(nnode.m_storage)
            nnode.remove();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param n 
     * @return true 
     * @return false 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline bool IntrusiveList<T, Container, member>::contains(const T& n) const
    {
        auto& nnode = n.*member;
        return nnode.m_storage == &m_storage;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return Container 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline Container IntrusiveList<T, Container, member>::first() const
    {
        return m_storage.m_first ? node_to_value(*m_storage.m_first) : nullptr;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return Container 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline Container IntrusiveList<T, Container, member>::take_first()
    {
        if(Container ptr = first())
        {
            remove(*ptr);
            return ptr;
        }
        return nullptr;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return Container 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline Container IntrusiveList<T, Container, member>::take_last()
    {
        if(Container ptr = last())
        {
            remove(*ptr);
            return ptr;
        }
        return nullptr;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return Container 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline Container IntrusiveList<T, Container, member>::last() const
    {
        return m_storage.m_last ? node_to_value(*m_storage.m_last) : nullptr;
    }   

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param current 
     * @return const T* 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline const T* IntrusiveList<T, Container, member>::next(const T* current)
    {
        auto& nextnode = (current->*member).m_next;
        const T* nextstruct = nextnode ? node_to_value(*nextnode) : nullptr;
        return nextstruct;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param current 
     * @return const T* 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline const T* IntrusiveList<T, Container, member>::prev(const T* current)
    {
        auto& prevnode = (current->*member).m_prev;
        const T* prevstruct = prevnode ? node_to_value(*prevnode) : nullptr;
        return prevstruct;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param current 
     * @return T* 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline T* IntrusiveList<T, Container, member>::next(T* current)
    {
        auto& nextnode = (current->*member).m_next;
        T* nextstruct = nextnode ? node_to_value(*nextnode) : nullptr;
        return nextstruct;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param current 
     * @return T* 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline T* IntrusiveList<T, Container, member>::prev(T* current)
    {
        auto& prevnode = (current->*member).m_prev;
        T* prevstruct = prevnode ? node_to_value(*prevnode) : nullptr;
        return prevstruct;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return IntrusiveList<T, Container, member>::Iterator 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline typename IntrusiveList<T, Container, member>::Iterator IntrusiveList<T, Container, member>::begin()
    {
        return m_storage.m_first ? Iterator(node_to_value(*m_storage.m_first)) : Iterator();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return IntrusiveList<T, Container, member>::ReverseIterator 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline typename IntrusiveList<T, Container, member>::ReverseIterator IntrusiveList<T, Container, member>::rbegin()
    {
        return m_storage.m_last ? ReverseIterator(node_to_value(*m_storage.m_last)) : ReverseIterator();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @return IntrusiveList<T, Container, member>::ConstIterator 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline typename IntrusiveList<T, Container, member>::ConstIterator IntrusiveList<T, Container, member>::begin() const
    {
        return m_storage.m_first ? ConstIterator(node_to_value(*m_storage.m_first)) : ConstIterator();
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @tparam member 
     * @param node 
     * @return T* 
     */
    template <class T, typename Container, SubstitutedIntrusiveListNode<T, Container> T::* member>
    inline T* IntrusiveList<T, Container, member>::node_to_value(SubstitutedIntrusiveListNode<T, Container>& node)
    {
        return bit_cast<T*>(bit_cast<unsigned char*>(&node) - bit_cast<unsigned char*>(member));
    }

    /**
     * @brief Destroy the Intrusive List Node< T,  Container>:: Intrusive List Node object
     * 
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container>
    inline IntrusiveListNode<T, Container>::~IntrusiveListNode()
    {
        VERIFY(!is_in_list());
    }

    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container>
    inline void IntrusiveListNode<T, Container>::remove()
    {
        VERIFY(m_storage);
        if(m_storage->m_first == this)
            m_storage->m_first = m_next;
        if(m_storage->m_last == this)
            m_storage->m_last = m_prev;
        if(m_prev)
            m_prev->m_next = m_next;
        if(m_next)
            m_next->m_prev = m_prev;
        m_prev = nullptr;
        m_next = nullptr;
        m_storage = nullptr;
        if constexpr(!IsRaw)
            m_self.reference = nullptr;
    }

    /**
     * @tparam T 
     * @tparam Container 
     * @return true 
     * @return false 
     */
    template <typename T, typename Container>
    inline bool IntrusiveListNode<T, Container>::is_in_list() const
    {
        return m_storage != nullptr;
    }

    /**
     * @tparam T 
     * @tparam member 
     */
    template <class T, SubstitutedIntrusiveListNode<T, NonnullRefPtr<T>> T::* member>
    class IntrusiveList<T, NonnullRefPtr<T>, member> : public IntrusiveList<T, RefPtr<T>, member>
    {
    public:
        /**
         * @return NonnullRefPtr<T> 
         */
        [[nodiscard]] NonnullRefPtr<T> first() const
        {
            return *IntrusiveList<T, RefPtr<T>, member>::first();
        }

        /**
         * @return NonnullRefPtr<T> 
         */
        [[nodiscard]] NonnullRefPtr<T> last() const
        {
            return *IntrusiveList<T, RefPtr<T>, member>::last();
        }

        /**
         * @return NonnullRefPtr<T> 
         */
        [[nodiscard]] NonnullRefPtr<T> take_first()
        {
            return *IntrusiveList<T, RefPtr<T>, member>::take_first();
        }

        /**
         * @return NonnullRefPtr<T> 
         */
        [[nodiscard]] NonnullRefPtr<T> take_last()
        {
            return *IntrusiveList<T, RefPtr<T>, member>::take_last();
        }
    }; // class IntrusiveList<T, NonnullRefPtr<T>, member> : public IntrusiveList<T, RefPtr<T>, member>
} // namespace Mods::Detail

namespace Mods
{
    /**
     * @tparam T 
     * @tparam Container 
     */
    template <typename T, typename Container = RawPtr<T>>
    using IntrusiveListNode = Detail::SubstitutedIntrusiveListNode<T, Container>;

    /**
     * @tparam member 
     */
    template <auto member>
    using IntrusiveList = Detail::IntrusiveList<
        decltype(Detail::ExtractIntrusiveListTypes::value(member)),
        decltype(Detail::ExtractIntrusiveListTypes::container(member)),
        member>;

} // namespace Mods

using Mods::IntrusiveList;
using Mods::IntrusiveListNode;
