/**
 * @file vector.h
 * @author Krisna Pranav
 * @brief Vector
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "forward.h"
#include "iterator.h"
#include "optional.h"
#include "span.h"
#include "stdlibextra.h"
#include "traits.h"
#include "typedtransfer.h"
#include "kmalloc.h"

#ifndef PRANAOS_LIBC_BUILD
#include <initializer_list>
#endif 

#ifndef __pranaos__
#include <new>
#endif

namespace Mods {
    template<typename T, size_t inline_capacity>
    class Vector {
    public:
        using value_type = T;

        Vector()
            : m_capacity(inline_capacity)
        {}

        ~Vector() {
            clear();
        } 

#ifndef PRANAOS_LIBC_BUILD
        Vector(std::initializer_list<T> list) {
            for (auto& item : list )
                unchecked_append(item);
        }
#endif

    private:
        void reset_capacity() {
            m_capacity = inline_capacity;
        }

        size_t m_size { 0 };
        size_t m_capacity { 0 };

        alignas(T) unsigned char m_inline_buffer_storage[sizeof(T) * inline_capacity];
        T* m_outline_buffer { nullptr };
    }; 
}