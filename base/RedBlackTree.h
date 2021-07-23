/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Concepts.h>

namespace Base {

template<Integral K>
class BaseRedBlackTree {
    BASE_MAKE_NONCOPYABLE(BaseRedBlackTree);
    BASE_MAKE_NONMOVABLE(BaseRedBlackTree);

public:
    [[nodiscard]] size_t size() const { return m_size; }
    [[nodiscard]] bool is_empty() const { return m_size == 0; }

    enum class Color : bool {
        Red,
        Black
    };
    struct Node {
        Node* left_child { nullptr };
        Node* right_child { nullptr };
        Node* parent { nullptr };

        Color color { Color::Red };

        K key;

        Node(K key)
            : key(key)
        {
        }
        virtual ~Node() {};
    };

protected:
    BaseRedBlackTree() = default; 
    virtual ~BaseRedBlackTree() {};

    void rotate_left(Node* subtree_root)
    {
        VERIFY(subtree_root);
        auto* pivot = subtree_root->right_child;
        VERIFY(pivot);
        auto* parent = subtree_root->parent;

        subtree_root->right_child = pivot->left_child;
        if (subtree_root->right_child)
            subtree_root->right_child->parent = subtree_root;

        pivot->left_child = subtree_root;
        subtree_root->parent = pivot;

        pivot->parent = parent;
        if (!parent) { 
            m_root = pivot;
        } else if (parent->left_child == subtree_root) { 
            parent->left_child = pivot;
        } else { 
            parent->right_child = pivot;
        }
    }

    void rotate_right(Node* subtree_root)
    {
        VERIFY(subtree_root);
        auto* pivot = subtree_root->left_child;
        VERIFY(pivot);
        auto* parent = subtree_root->parent;

        subtree_root->left_child = pivot->right_child;
        if (subtree_root->left_child)
            subtree_root->left_child->parent = subtree_root;

        pivot->right_child = subtree_root;
        subtree_root->parent = pivot;

        pivot->parent = parent;
        if (!parent) { 
            m_root = pivot;
        } else if (parent->left_child == subtree_root) {
            parent->left_child = pivot;
        } else { 
            parent->right_child = pivot;
        }
    }

    static Node* find(Node* node, K key)
    {
        while (node && node->key != key) {
            if (key < node->key) {
                node = node->left_child;
            } else {
                node = node->right_child;
            }
        }
        return node;
    }

    static Node* find_largest_not_above(Node* node, K key)
    {
        Node* candidate = nullptr;
        while (node) {
            if (key == node->key) {
                return node;
            } else if (key < node->key) {
                node = node->left_child;
            } else {
                candidate = node;
                node = node->right_child;
            }
        }
        return candidate;
    }

    void insert(Node* node)
    {
        VERIFY(node);
        Node* parent = nullptr;
        Node* temp = m_root;
        while (temp) {
            parent = temp;
            if (node->key < temp->key) {
                temp = temp->left_child;
            } else {
                temp = temp->right_child;
            }
        }
        if (!parent) { 
            node->color = Color::Black;
            m_root = node;
            m_size = 1;
            m_minimum = node;
            return;
        } else if (node->key < parent->key) { 
            parent->left_child = node;
        } else { 
            parent->right_child = node;
        }
        node->parent = parent;

        if (node->parent->parent) 
            insert_fixups(node);

        m_size++;
        if (m_minimum->left_child == node)
            m_minimum = node;
    }

    void insert_fixups(Node* node)
    {
        VERIFY(node && node->color == Color::Red);
        while (node->parent && node->parent->color == Color::Red) {
            auto* grand_parent = node->parent->parent;
            if (grand_parent->right_child == node->parent) {
                auto* uncle = grand_parent->left_child;
                if (uncle && uncle->color == Color::Red) {
                    node->parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grand_parent->color = Color::Red;
                    node = grand_parent;
                } else {
                    if (node->parent->left_child == node) {
                        node = node->parent;
                        rotate_right(node);
                    }
                    node->parent->color = Color::Black;
                    grand_parent->color = Color::Red;
                    rotate_left(grand_parent);
                }
            } else {
                auto* uncle = grand_parent->right_child;
                if (uncle && uncle->color == Color::Red) {
                    node->parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grand_parent->color = Color::Red;
                    node = grand_parent;
                } else {
                    if (node->parent->right_child == node) {
                        node = node->parent;
                        rotate_left(node);
                    }
                    node->parent->color = Color::Black;
                    grand_parent->color = Color::Red;
                    rotate_right(grand_parent);
                }
            }
        }
        m_root->color = Color::Black; 
    }

    void remove(Node* node)
    {
        VERIFY(node);

        if (m_size == 1) {
            m_root = nullptr;
            m_size = 0;
            return;
        }

        if (m_minimum == node)
            m_minimum = successor(node);

        if (node->left_child && node->right_child) {
            auto* successor_node = successor(node); 
            auto neighbour_swap = successor_node->parent == node;
            node->left_child->parent = successor_node;
            if (!neighbour_swap)
                node->right_child->parent = successor_node;
            if (node->parent) {
                if (node->parent->left_child == node) {
                    node->parent->left_child = successor_node;
                } else {
                    node->parent->right_child = successor_node;
                }
            } else {
                m_root = successor_node;
            }
            if (successor_node->right_child)
                successor_node->right_child->parent = node;
            if (neighbour_swap) {
                successor_node->parent = node->parent;
                node->parent = successor_node;
            } else {
                if (successor_node->parent) {
                    if (successor_node->parent->left_child == successor_node) {
                        successor_node->parent->left_child = node;
                    } else {
                        successor_node->parent->right_child = node;
                    }
                } else {
                    m_root = node;
                }
                swap(node->parent, successor_node->parent);
            }
            swap(node->left_child, successor_node->left_child);
            if (neighbour_swap) {
                node->right_child = successor_node->right_child;
                successor_node->right_child = node;
            } else {
                swap(node->right_child, successor_node->right_child);
            }
            swap(node->color, successor_node->color);
        }

        auto* child = node->left_child ?: node->right_child;

        if (child)
            child->parent = node->parent;
        if (node->parent) {
            if (node->parent->left_child == node)
                node->parent->left_child = child;
            else
                node->parent->right_child = child;
        } else {
            m_root = child;
        }

        if (node->color != Color::Red)
            remove_fixups(child, node->parent);

        m_size--;
    }

    void remove_fixups(Node* node, Node* parent)
    {
        while (node != m_root && (!node || node->color == Color::Black)) {
            if (parent->left_child == node) {
                auto* sibling = parent->right_child;
                if (sibling->color == Color::Red) {
                    sibling->color = Color::Black;
                    parent->color = Color::Red;
                    rotate_left(parent);
                    sibling = parent->right_child;
                }
                if ((!sibling->left_child || sibling->left_child->color == Color::Black) && (!sibling->right_child || sibling->right_child->color == Color::Black)) {
                    sibling->color = Color::Red;
                    node = parent;
                } else {
                    if (!sibling->right_child || sibling->right_child->color == Color::Black) {
                        sibling->left_child->color = Color::Black; 
                        sibling->color = Color::Red;
                        rotate_right(sibling);
                        sibling = parent->right_child;
                    }
                    sibling->color = parent->color;
                    parent->color = Color::Black;
                    sibling->right_child->color = Color::Black; 
                    rotate_left(parent);
                    node = m_root; 
                }
            } else {
                auto* sibling = parent->left_child;
                if (sibling->color == Color::Red) {
                    sibling->color = Color::Black;
                    parent->color = Color::Red;
                    rotate_right(parent);
                    sibling = parent->left_child;
                }
                if ((!sibling->left_child || sibling->left_child->color == Color::Black) && (!sibling->right_child || sibling->right_child->color == Color::Black)) {
                    sibling->color = Color::Red;
                    node = parent;
                } else {
                    if (!sibling->left_child || sibling->left_child->color == Color::Black) {
                        sibling->right_child->color = Color::Black; 
                        sibling->color = Color::Red;
                        rotate_left(sibling);
                        sibling = parent->left_child;
                    }
                    sibling->color = parent->color;
                    parent->color = Color::Black;
                    sibling->left_child->color = Color::Black; 
                    rotate_right(parent);
                    node = m_root; 
                }
            }
            parent = node->parent;
        }
        node->color = Color::Black; 
    }

    static Node* successor(Node* node)
    {
        VERIFY(node);
        if (node->right_child) {
            node = node->right_child;
            while (node->left_child)
                node = node->left_child;
            return node;
        } else {
            auto temp = node->parent;
            while (temp && node == temp->right_child) {
                node = temp;
                temp = temp->parent;
            }
            return temp;
        }
    }

    static Node* predecessor(Node* node)
    {
        VERIFY(node);
        if (node->left_child) {
            node = node->left_child;
            while (node->right_child)
                node = node->right_child;
            return node;
        } else {
            auto temp = node->parent;
            while (temp && node == temp->left_child) {
                node = temp;
                temp = temp->parent;
            }
            return temp;
        }
    }

    Node* m_root { nullptr };
    size_t m_size { 0 };
    Node* m_minimum { nullptr }; 
};

template<typename TreeType, typename ElementType>
class RedBlackTreeIterator {
public:
    RedBlackTreeIterator() = default;
    bool operator!=(const RedBlackTreeIterator& other) const { return m_node != other.m_node; }
    RedBlackTreeIterator& operator++()
    {
        if (!m_node)
            return *this;
        m_prev = m_node;
        m_node = static_cast<typename TreeType::Node*>(TreeType::successor(m_node));
        return *this;
    }
    RedBlackTreeIterator& operator--()
    {
        if (!m_prev)
            return *this;
        m_node = m_prev;
        m_prev = static_cast<typename TreeType::Node*>(TreeType::predecessor(m_prev));
        return *this;
    }
    ElementType& operator*() { return m_node->value; }
    ElementType* operator->() { return &m_node->value; }
    [[nodiscard]] bool is_end() const { return !m_node; }
    [[nodiscard]] bool is_begin() const { return !m_prev; }

    [[nodiscard]] auto key() const { return m_node->key; }

private:
    friend TreeType;
    explicit RedBlackTreeIterator(typename TreeType::Node* node, typename TreeType::Node* prev = nullptr)
        : m_node(node)
        , m_prev(prev)
    {
    }
    typename TreeType::Node* m_node { nullptr };
    typename TreeType::Node* m_prev { nullptr };
};

template<Integral K, typename V>
class RedBlackTree final : public BaseRedBlackTree<K> {
public:
    RedBlackTree() = default;
    virtual ~RedBlackTree() override
    {
        clear();
    }

    using BaseTree = BaseRedBlackTree<K>;

    [[nodiscard]] V* find(K key)
    {
        auto* node = static_cast<Node*>(BaseTree::find(this->m_root, key));
        if (!node)
            return nullptr;
        return &node->value;
    }

    [[nodiscard]] V* find_largest_not_above(K key)
    {
        auto* node = static_cast<Node*>(BaseTree::find_largest_not_above(this->m_root, key));
        if (!node)
            return nullptr;
        return &node->value;
    }

    void insert(K key, const V& value)
    {
        insert(key, V(value));
    }

    [[nodiscard]] bool try_insert(K key, V&& value)
    {
        auto* node = new (nothrow) Node(key, move(value));
        if (!node)
            return false;
        BaseTree::insert(node);
        return true;
    }

    void insert(K key, V&& value)
    {
        auto success = try_insert(key, move(value));
        VERIFY(success);
    }

    using Iterator = RedBlackTreeIterator<RedBlackTree, V>;
    friend Iterator;
    Iterator begin() { return Iterator(static_cast<Node*>(this->m_minimum)); }
    Iterator end() { return {}; }
    Iterator begin_from(K key) { return Iterator(static_cast<Node*>(BaseTree::find(this->m_root, key))); }

    using ConstIterator = RedBlackTreeIterator<const RedBlackTree, const V>;
    friend ConstIterator;
    ConstIterator begin() const { return ConstIterator(static_cast<Node*>(this->m_minimum)); }
    ConstIterator end() const { return {}; }
    ConstIterator begin_from(K key) const { return ConstIterator(static_cast<Node*>(BaseTree::find(this->m_root, key))); }

    ConstIterator find_largest_not_above_iterator(K key) const
    {
        auto node = static_cast<Node*>(BaseTree::find_largest_not_above(this->m_root, key));
        return ConstIterator(node, static_cast<Node*>(BaseTree::predecessor(node)));
    }

    V unsafe_remove(K key)
    {
        auto* node = BaseTree::find(this->m_root, key);
        VERIFY(node);

        BaseTree::remove(node);

        V temp = move(static_cast<Node*>(node)->value);

        node->right_child = nullptr;
        node->left_child = nullptr;
        delete node;

        return temp;
    }

    bool remove(K key)
    {
        auto* node = BaseTree::find(this->m_root, key);
        if (!node)
            return false;

        BaseTree::remove(node);

        node->right_child = nullptr;
        node->left_child = nullptr;
        delete node;

        return true;
    }

    void clear()
    {
        if (this->m_root) {
            delete this->m_root;
            this->m_root = nullptr;
        }
        this->m_minimum = nullptr;
        this->m_size = 0;
    }

private:
    struct Node : BaseRedBlackTree<K>::Node {

        V value;

        Node(K key, V value)
            : BaseRedBlackTree<K>::Node(key)
            , value(move(value))
        {
        }

        ~Node()
        {
            if (this->left_child)
                delete this->left_child;
            if (this->right_child)
                delete this->right_child;
        }
    };
};

}

using Base::RedBlackTree;