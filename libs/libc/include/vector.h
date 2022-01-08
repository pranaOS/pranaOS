#pragma once

#include <types.h>

namespace pranaOSVector {
    template <typename T>
    class Vector {
    public:
        Vector() {
            this->size = 0;
            this->capacity = 0;
            this->buffer = 0;
        }

        Vector() {
            this->clear();
        }

        int size() {
            return this->size;
        }

        void push_back(const T& item) {

        }

        void pop_back() {
            this->size--;
        }

        void clear() {
            
        }

        T& getAt(int n) {
            return this->buffer[n];
        }

        T* data() {
            return this->buffer;
        }

        typedef T* iterator;
        iterator begin() {
            return this->buffer;
        }

        iterator end() {
            return this->buffer + this->size;
        }
    };
}