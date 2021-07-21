/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RedBlackTree.h>

namespace Base {

template<Integral K>
class IntrusiveRedBlackTreeNode;

template<Integral K, typename V, IntrusiveRedBlackTreeNode<K> V::*member>
class IntrusiveRedBlackTree final : public BaseRedBlackTree<K> {

public:
    IntrusiveRedBlackTree() = default;
    virtual ~IntrusiveRedBlackTree() override
    {
        clear();
    }

    using BaseTree = BaseRedBlackTree<K>;
    using TreeNode = IntrusiveRedBlackTreeNode<K>;

    V* find(K key)
    {
        auto* node = static_cast<TreeNode*>(BaseTree::find(this->m_root, key));
        if (!node)
            return nullptr;
        return node_to_value(*node);
    }

    V* find_largest_not_above(K key)
    {
        auto* node = static_cast<TreeNode*>(BaseTree::find_largest_not_above(this->m_root, key));
        if (!node)
            return nullptr;
        return node_to_value(*node);
    }

    void insert(V& value)
    {
        auto& node = value.*member;
        BaseTree::insert(&node);
        node.m_in_tree = true;
    }

        template<typename ElementType>
    class BaseIterator {
    public:
        BaseIterator() = default;
        bool operator!=(const BaseIterator& other) const { return m_node != other.m_node; }
        BaseIterator& operator++()
        {
            if (!m_node)
                return *this;
            m_prev = m_node;
            // the complexity is O(logn) for each successor call, but the total complexity for all elements comes out to O(n), meaning the amortized cost for a single call is O(1)
            m_node = static_cast<TreeNode*>(BaseTree::successor(m_node));
            return *this;
        }
        BaseIterator& operator--()
        {
            if (!m_prev)
                return *this;
            m_node = m_prev;
            m_prev = static_cast<TreeNode*>(BaseTree::predecessor(m_prev));
            return *this;
        }
        ElementType& operator*()
        {
            VERIFY(m_node);
            return *node_to_value(*m_node);
        }
        ElementType* operator->()
        {
            VERIFY(m_node);
            return node_to_value(*m_node);
        }
        [[nodiscard]] bool is_end() const { return !m_node; }
        [[nodiscard]] bool is_begin() const { return !m_prev; }
        [[nodiscard]] auto key() const { return m_node->key; }

    private:
        friend class IntrusiveRedBlackTree;
        explicit BaseIterator(TreeNode* node, TreeNode* prev = nullptr)
            : m_node(node)
            , m_prev(prev)
        {
        }
        TreeNode* m_node { nullptr };
        TreeNode* m_prev { nullptr };
    };

}
}
