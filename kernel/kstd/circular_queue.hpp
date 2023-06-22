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
            
        }
    };
}