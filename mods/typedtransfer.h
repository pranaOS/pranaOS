/**
 * @file typedtransfer.h
 * @author Krisna Pranav
 * @brief TypedTransfer
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "traits.h"

// mods

namespace Mods {

    template<typename T>
    class TypedTransfer {
    public:
        /**
         * @brief move
         * 
         * @param destination 
         * @param source 
         * @param count 
         * @return size_t 
         */
        static size_t move(T* destination, T* source, size_t count) {
            if constexpr (Traits<T>::is_trivial()) {
                __builtin_memmove(destination, source, count * sizeof(T));
                return count;
            }

            for (size_t i = 0; i < count; ++i) {
                if (destination <= source)
                    new (&destination[i]) T(Mods::move(source[i]));
                else
                    new (&destination[count - i - 1]) T(Mods::move(source[count - i - 1]));
            }

            return count;
        }

        /**
         * @brief copy
         * 
         * @param destination 
         * @param source 
         * @param count 
         * @return size_t 
         */
        static size_t copy(T* destination, const T* source, size_t count) {
            if constexpr (Traits<T>::is_trivial()) {
                __builtin_memmove(destination, source, count * sizeof(T));
                return count;
            }

            for (size_t i = 0; i < count; ++i) {
                if (destination <= source)
                    new (&destination[i]) T(source[i]);
                else
                    new (&destination[count - i - 1]) T(source[count - i - 1]);
            }

            return count;
        }

        /**
         * @brief compare
         * 
         * @param a 
         * @param b 
         * @param count 
         * @return true 
         * @return false 
         */
        static bool compare(const T* a, const T* b, size_t count) {
            if constexpr (Traits<T>::is_trivial())
                return !__builtin_memcmp(a, b, count * sizeof(T));

            for (size_t i = 0; i < count; ++i) {
                if (a[i] != b[i])
                    return false;
            }

            return true;
        }
    };

}

// using mods