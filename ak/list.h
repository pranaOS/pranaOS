#pragma once

#include <system/tasking/lock.h>

namespace pranaOS {
    namespace ak {
        template <typename T>
        struct ListNode {
            ListNode(const T &e) : data(e), next(0)
            {}

            T data;
            ListNode<T>* next;
        };

        template <typename T>
        class List {
        public:
            List() : head_(0), tail_(0), size_(0)
            {}

            ~List()
            {
                this->clear();
            }

            int size() { 
                return size_;
            }

            void push_back(const T &e)
            void push_front(const T &e);
            void clear();

            T getat(int index);
            T operator[](int index);
            int indexof(const T &e);

            void remove(int index);
            void remove(const T &e);
        }
    }
}