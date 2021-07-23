/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Concepts.h>

namespace Base {


template<Intergral K>
class BaseRedBlackTree {
    BASE_MAKE_NONCOPYABLE(BaseRedBlackTree);
    BASE_MAKE_NONCOPYABLE(BaseRedBlackTree);

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
    virtual ~BaseRedBlackTree() {}
    
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

}

}
