/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/assert.h>
#include <libutils/iteration.h>
#include <libutils/optional.h>
#include <libutils/prelude.h>

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

        Node(const T &v) : value {v} {}
    };

    size_t count = 0;
    Node *_head = nullptr;
    Node *_tail = nullptr;


};

}