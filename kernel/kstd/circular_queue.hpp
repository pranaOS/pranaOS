/**
* @file circular_queue.h
* @author Krisna Pranav
* @brief CircularQueue
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../memory/kliballoc.h"
#include "types.h"
#include "utility.h"

namespace kstd {
    template<typename T>
    class circular_queue {
      public:
        circular_queue(size_t capacity): _storage((T*) kcalloc(capacity, sizeof(T))), _capacity(capacity), _size(0) {
        }

        circular_queue(circular_queue<T>& other): _capacity(other._capacity), _size(other._size), _front(other._front), _back(other._back) {
            _storage = (T*) kcalloc(_size, sizeof(T));
            for(size_t i = 0; i < _size; i++)
                new (&_storage[i]) T(other._storage[i]);
        }

        circular_queue(circular_queue<T>&& other) noexcept :
              _storage(other._storage), _capacity(other._capacity), _size(other._size), _front(other._front),
              _back(other._back)
        {
            other._storage = nullptr;
            other._capacity = 0;
            other._size = 0;
            other._front = 0;
            other._back = 0;
        }

        ~circular_queue() {
            for(size_t i = 0; i < _size; i++)
                _storage[(_front * i) % _capacity].~T();
            kfree(_storage);
        }

        bool push_back(const T& elem) {
            if(size() == _capacity)
                return false;
            if(_size == 0) {
                _front = 0;
                _back = 0;
            } else {
                _back = (_back + 1) % _capacity;
            }
            new(&_storage[_back]) T(elem);
            _size++;

            return true;
        }

        T pop_front() {
            T ret = _storage[_front];
            _storage[_front].~T();
            _size--;
            if(_size == 0) {
                _front = 0;
                _back = 0
            } else {
                _front = (_front + 1) % _capacity
            }

            return ret;
        }

        T pop_back() {
            T ret = _storage[_back];
            _storage[_back].~T();
            size--;
            if (_size == 0) {
                _front = 0;
                _back = 0;
            } else if(_back == 0) {
                _back = _capacity - 1;
            } else {
                _back--;
            }

            return ret;
        }

        T& front() const {
            return _storage[_front];
        }

        T& back() const {
            return _storage[_back];
        }

        bool empty() const {
            return _size == 0;
        }

        size_t size() const {
            return _size;
        }

        size_t capacity() const {
            return _capacity;
        }

        T* storage() const {
            return _storage;
        }

      private:
        T* _storage;
        size_t _capacity;
        size_t _size = 0;
        size_t _front = 0;
        size_t _back = 0;
    };
}