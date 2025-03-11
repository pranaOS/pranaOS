#pragma once

#include "Common.hpp"
#include "Runtime.hpp"

template <typename ListType, typename ValueType>
class ListIterator {
    friend ListType;

public:
    explicit ListIterator(typename ListType::Node* node_ptr = nullptr)
        : m_node_ptr(node_ptr)
    {
    }

    const ValueType& operator*() const { return m_node_ptr->val; }
    ValueType& operator*() { return m_node_ptr->val; }

    ValueType* operator->() { return &m_node_ptr->val; }
    ValueType const* operator->() const { return &m_node_ptr->val; }

    bool operator==(const ListIterator& it) const { return m_node_ptr == it.m_node_ptr; }
    bool operator!=(const ListIterator& it) const { return m_node_ptr != it.m_node_ptr; }

    operator bool() { return m_node_ptr != nullptr; }

    ListIterator operator++()
    {
        m_node_ptr = m_node_ptr->next;
        return *this;
    }
    ListIterator operator--()
    {
        m_node_ptr = m_node_ptr->prev;
        return *this;
    }

    ListIterator operator++(int)
    {
        auto cp = *this;
        m_node_ptr = m_node_ptr->next;
        return cp;
    }
    ListIterator operator--(int)
    {
        auto cp = *this;
        m_node_ptr = m_node_ptr->prev;
        return cp;
    }

private:
    typename ListType::Node* m_node_ptr;
};

template <typename T>
class List {
public:
    using ValueType = T;

    struct Node {
        explicit Node(const T& val)
            : val(val)
        {
        }

        explicit Node(T&& val)
            : val(move(val))
        {
        }

        T val;
        Node* next {};
        Node* prev {};
    };

    List() = default;
    ~List() { clear(); }
    List(const List& list);
    List& operator=(const List& list);

    void push_front(const T& val) { push_front(new Node(val)); }
    void push_front(T&& val) { push_front(new Node(move(val))); }
    void push_back(const T& val) { push_back(new Node(val)); }
    void push_back(T&& val) { push_back(new Node(move(val))); }

    void pop_front()
    {
        remove(begin());
    }

    void pop_back()
    {
        remove(rbegin());
    }

    void clear();
    bool empty() const { return !m_size; }
    size_t size() const { return m_size; }

    using Iterator = ListIterator<List, ValueType>;
    friend Iterator;

    Iterator begin() { return Iterator(m_head); }
    Iterator rbegin() { return Iterator(m_tail); }
    Iterator end() { return Iterator(nullptr); }
    Iterator rend() { return Iterator(nullptr); }

    using ConstIterator = ListIterator<const List, const T>;
    friend ConstIterator;

    ConstIterator begin() const { return ConstIterator(m_head); }
    ConstIterator rbegin() const { return ConstIterator(m_tail); }
    ConstIterator end() const { return ConstIterator(nullptr); }
    ConstIterator rend() const { return ConstIterator(nullptr); }

    Iterator find(const ValueType& value);
    ConstIterator find(const ValueType& value) const;

    Iterator remove(const Iterator& del_it);

    void append(const Iterator& it_begin, const Iterator& it_end);

    ValueType& back() { return *rbegin(); }
    const ValueType& back() const { return *rbegin(); }

    ValueType& front() { return *begin(); }
    const ValueType& front() const { return *begin(); }

    Iterator insert_before(Iterator it, const T& val)
    {
        auto new_node = new Node(val);
        insert_before(it.m_node_ptr, new_node);
        return Iterator(new_node);
    }

    Iterator insert_before(Iterator it, T&& val)
    {
        auto new_node = new Node(move(val));
        insert_before(it.m_node_ptr, new_node);
        return Iterator(new_node);
    }

    Iterator insert_after(Iterator it, const T& val)
    {
        auto new_node = new Node(val);
        insert_after(it.m_node_ptr, new_node);
        return Iterator(new_node);
    }

    Iterator insert_after(Iterator it, T&& val)
    {
        auto new_node = new Node(move(val));
        insert_after(it.m_node_ptr, new_node);
        return Iterator(new_node);
    }

private:
    void push_front(Node* node);
    void push_back(Node* node);
    void insert_before(Node* node, Node* new_node);
    void insert_after(Node* node, Node* new_node);

private:
    Node* m_head {};
    Node* m_tail {};

    size_t m_size {};
};

template <typename T>
void List<T>::push_front(Node* node)
{
    m_size++;

    if (!m_head) {
        m_head = node;
        m_tail = node;
        return;
    }

    m_head->prev = node;
    node->next = m_head;
    m_head = node;
}

template <typename T>
void List<T>::push_back(Node* node)
{
    m_size++;

    if (!m_head) {
        m_head = node;
        m_tail = node;
        return;
    }

    m_tail->next = node;
    node->prev = m_tail;
    m_tail = node;
}

template <typename T>
void List<T>::clear()
{
    auto it = rbegin();
    while (it != rend()) {
        it = remove(it);
    }
}

template <typename T>
typename List<T>::ConstIterator List<T>::find(const ValueType& value) const
{
    for (auto it = this->begin(); it != this->end(); ++it) {
        if (*it == value) {
            return it;
        }
    }

    return end();
}

template <typename T>
typename List<T>::Iterator List<T>::find(const ValueType& value)
{
    for (auto it = this->begin(); it != this->end(); ++it) {
        if (*it == value) {
            return it;
        }
    }

    return end();
}

template <typename T>
typename List<T>::Iterator List<T>::remove(const List::Iterator& del_it)
{
    auto node_ptr = del_it.m_node_ptr;

    if (node_ptr == m_head) {
        m_head = node_ptr->next;
    }
    if (node_ptr == m_tail) {
        m_tail = node_ptr->prev;
    }

    if (node_ptr->prev) {
        node_ptr->prev->next = node_ptr->next;
    }
    if (node_ptr->next) {
        node_ptr->next->prev = node_ptr->prev;
    }

    auto prev = del_it;
    --prev;
    delete node_ptr;
    m_size--;
    return prev;
}

template <typename T>
List<T>::List(const List& list)
{
    for (const auto& el : list) {
        push_back(el);
    }
}

template <typename T>
List<T>& List<T>::operator=(const List& list)
{
    clear();
    for (const auto& el : list) {
        push_back(el);
    }
    return *this;
}

template <typename T>
void List<T>::append(const Iterator& it_begin, const Iterator& it_end)
{
    for (auto it = it_begin; it != it_end; it++) {
        push_back(*it);
    }
}

template <typename T>
void List<T>::insert_before(List<T>::Node* node, List<T>::Node* new_node)
{
    if (!node) {
        push_back(new_node);
        return;
    }

    m_size++;

    if (node == m_head) {
        m_head->prev = new_node;
        new_node->next = m_head;
        m_head = new_node;
        return;
    }

    if (node->prev) {
        node->prev->next = new_node;
        new_node->prev = node->prev;
    }

    node->prev = new_node;
    new_node->next = node;
}

template <typename T>
void List<T>::insert_after(List<T>::Node* node, List<T>::Node* new_node)
{
    if (!node) {
        push_front(new_node);
        return;
    }

    m_size++;

    if (node == m_tail) {
        m_tail->next = new_node;
        new_node->prev = m_tail;
        m_tail = new_node;
        return;
    }

    if (node->next) {
        node->next->prev = new_node;
        new_node->next = node->next;
    }

    node->next = new_node;
    new_node->prev = node;
}