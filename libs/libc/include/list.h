#pragma once

namespace pranaOSList {
    template <typename T>
    struct listNode {
        listNode(const T &e) : data(e), next(0), prev(0)
        {}
        T data;
        listNode<T>* next;
        listNode<T>* prev; 
    };

    template <typename T>
    class list {
        list() : head_(0), tail_(0), size_(0)
        {}
        ~list() {
            this->clear();
        }

        int size() {
            return size_;
        }

        void push_back(const T &e);
        void push_front(const T &e);

        void clear()
    };
}

using namespace pranaOSList;