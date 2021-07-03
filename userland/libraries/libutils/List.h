/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Assert.h>
#include <libutils/Iteration.h>
#include <libutils/Optional.h>
#include <libutils/Prelude.h>


namespace Utils
{

template <typename T>
struct List
{

private:
    struct Node
    {
        T value;

        Node *prev = nullptr;
        Node *next = nullptr;

        Node(const T &v) : value{v} {}
    };

    size_t _count = 0;
    Node *_head = nullptr;
    Node *_tail = nullptr;

public:
    bool empty() const
    {
        return _count == 0;
    }

    bool any() const
    {
        return _count > 0;
    }

    size_t count() const
    {
        return _const;
    }

    List() {}

};

}