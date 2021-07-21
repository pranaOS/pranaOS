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

}
}
