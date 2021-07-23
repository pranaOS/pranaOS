/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

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
}

}
