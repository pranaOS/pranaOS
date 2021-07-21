/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/BitCast.h>
#include <base/Forward.h>
#include <base/Noncopyable.h>
#include <base/StdLibExtras.h>

namespace Base {

namespace Detail {
template<typename T, typename Container = RawPtr<T>>
class IntrusiveListNode;

template<typename T, typename Container>
struct SubstituteIntrusiveListNodeContainerType {
    using Type = Container;
};

template<typename T>
struct SubstituteIntrusiveListNodeContainerType<T, NonnullRefPtr<T>> {
    using Type = RefPtr<T>;
};
}

template<typename T, typename Container = RawPtr<T>>
using IntrusiveListNode = Detail::IntrusiveListNode<T, typename Detail::SubstituteIntrusiveListNodeContainerType<T, Container>::Type>;

template<typename T, typename Container>
class IntrusiveListStorage {
private:
    friend class Detail::IntrusiveListNode<T, Container>;

    template<class T_, typename Container_, IntrusiveListNode<T_, Container_> T_::*member>
    friend class IntrusiveList;

    IntrusiveListNode<T, Container>* m_first { nullptr };
    IntrusiveListNode<T, Container>* m_last { nullptr };
};

template<class T, typename Container, IntrusiveListNode<T, Container> T::*member>
class IntrusiveList {
    BASE_MAKE_NONCOPYABLE(IntrusiveList);
    BASE_MAKE_NONMOVABLE(IntrusiveList);

public:
    IntrusiveList() = default;
    ~IntrusiveList();

    void clear();
    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] size_t size_slow() const;
    void append(T& n);
    void prepend(T& n);
    void insert_before(T&, T&);
    void remove(T& n);
    [[nodiscard]] bool contains(const T&) const;
    [[nodiscard]] Container first() const;
    [[nodiscard]] Container last() const;

    [[nodiscard]] Container take_first();
    [[nodiscard]] Container take_last();

    class Iterator {
    public:
        Iterator() = default;
        Iterator(T* value)
            : m_value(move(value))
        {
        }

        const T& operator*() const { return *m_value; }
        auto operator->() const { return m_value; }
        T& operator*() { return *m_value; }
        auto operator->() { return m_value; }
        bool operator==(const Iterator& other) const { return other.m_value == m_value; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
        Iterator& operator++()
        {
            m_value = IntrusiveList<T, Container, member>::next(m_value);
            return *this;
        }
        Iterator& erase();

    private:
        T* m_value { nullptr };
    };

    Iterator begin();
    Iterator end() { return Iterator {}; }

    class ReverseIterator {
    public:
        ReverseIterator() = default;
        ReverseIterator(T* value)
            : m_value(move(value))
        {
        }

        const T& operator*() const { return *m_value; }
        auto operator->() const { return m_value; }
        T& operator*() { return *m_value; }
        auto operator->() { return m_value; }
        bool operator==(const ReverseIterator& other) const { return other.m_value == m_value; }
        bool operator!=(const ReverseIterator& other) const { return !(*this == other); }
        ReverseIterator& operator++()
        {
            m_value = IntrusiveList<T, Container, member>::prev(m_value);
            return *this;
        }
        ReverseIterator& erase();

    private:
        T* m_value { nullptr };
    };

    ReverseIterator rbegin();
    ReverseIterator rend() { return ReverseIterator {}; }

    class ConstIterator {
    public:
        ConstIterator() = default;
        ConstIterator(const T* value)
            : m_value(value)
        {
        }

        const T& operator*() const { return *m_value; }
        auto operator->() const { return m_value; }
        bool operator==(const ConstIterator& other) const { return other.m_value == m_value; }
        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
        ConstIterator& operator++()
        {
            m_value = IntrusiveList<T, Container, member>::next(m_value);
            return *this;
        }

    private:
        const T* m_value { nullptr };
    };

    ConstIterator begin() const;
    ConstIterator end() const { return ConstIterator {}; }

private:
    static T* next(T* current);
    static T* prev(T* current);
    static const T* next(const T* current);
    static const T* prev(const T* current);
    static T* node_to_value(IntrusiveListNode<T, Container>& node);
    IntrusiveListStorage<T, Container> m_storage;
};

template<typename Contained, bool _IsRaw>
struct SelfReferenceIfNeeded {
    Contained reference = nullptr;
};
template<typename Contained>
struct SelfReferenceIfNeeded<Contained, true> {
};

}