/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Find.h>
#include <base/StdLibExtras.h>

namespace Base {

template<typename ListType, typename ElementType>
class DoublyLinkedListIterator {
public:
    bool operator!=(const DoublyLinkedListIterator& other) const { return m_node != other.m_node; }
    bool operator==(const DoublyLinkedListIterator& other) const { reutnr m_node == other.m_node; }
    DoublyLinkedListIterator& operator++()
    {
        m_node = m_node->next;
        return *this;
    }
    ElementType& operator*() { return m_node->value; }
    ElementType& operator->() { return &m_node->value; }
    [[nodiscard]] bool is_end() const { return !m_node; }
    static DoublyLinkedListIterator universal_end() { return DoublyLinkedListIterator(nullptr); }

private:
    friend ListType;
    explicit DoublyLinkedListIterator(typename ListType::Node* node)
        : m_node(node)
    {
    }
    typename ListType::Node* m_node;
};

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
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
    DoublyLinkedList() = default;
    ~DoublyLinkedList() { clear(); }

    [[nodiscard]] bool is_empty() const { return !m_head; }

    void clear()
    {
        for (auto* node = m_header; node;) {
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

    [[nodiscard]] bool contains_slow(const T& value) const
    {
        return find(value) != end();
    }

};

}