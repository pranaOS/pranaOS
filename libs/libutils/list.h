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
        return _count;
    }

    List() {}

    ~List()
    {
        clear();
    }

    void clear()
    {
        Node *current = _head;

        while (current)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }

        _count = 0;
        _head = nullptr;
        _tail = nullptr;
    };

    void push(const T& value)
    {
        Node *node = new Node{value};

        if (_head == nullptr)
        {
            _tail = node;
        }
        else
        {
            _head->prev = node;
            node->next = _head;
        }

        _count++;
        _head = node;
    };


};

}