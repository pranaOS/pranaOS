/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <tasking/lock.h>

namespace ak {
    template <typename T>
    struct ListNode {
        ListNode(const T &e) : data(e), next(0), prev(0)
        {}

        T data;
        ListNode<T>* next;
        ListNode<T>* prev;
    };

    template <typename T>
    class List {
    public:
        List() : head_(0), tail_(0), size_(0)
        {}

        ~List() {
            this->clear();
        }

        int size() {
            return size_;
        }

        void push_back(const T &e);
        void push_front(const T &e);
        void clear();

        T getat(int index);
        T operator[](int index);
        int indexof(const T &e);

        void remove(int index);
        void remove(const T &e);

        private:
            ListNode<T>* head_;
            ListNode<T>* tail_;
            system::mutexLock lock;

            int size_;

            ListNode<T>* insertInternal(const T &e, ListNode<T>* pos);
            void removeInternal(ListNode<T> *pos);

        public:
            class iterator {
            public:
                iterator(ListNode<T> *p=0) : pos_(p) { }
                
                T &operator*() {
                    return pos_->data;
                }
    
                T *operator->() {
                    return &(pos_->data);
                }
    
                bool operator!=(const iterator &rhs) {
                    return this->pos_ != rhs.pos_;
                }
    
                iterator operator++() {
                    pos_ = pos_->next; return *this;
                }
    
                iterator operator--() {
                    pos_ = pos_->prev; return *this;
                }
    
            private:
                ListNode<T> *pos_;
            };

            iterator begin()
            {
                return iterator(head_);
            }

            iterator end()
            {
                return iterator(0);
            }
        };
    }
}

using namespace ak;

template <typename T>
ListNode<T>* List<T>::insertInternal(const T& e, ListNode<T>* pos) {
    this->lock.lock();
    ListNode<T>* List<T>::insertInternal(const T &e, ListNode<T>* pos)
    size_++;

    n->next = pos;

    if (pos) {
        n->prev = pos->prev;
        pos->prev = n;
    } else {
        n->prev = tail_;
        tail_ = n;
    }

    this->lock.unlock();
    return n;
}