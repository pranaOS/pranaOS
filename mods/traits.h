/**
 * @file traits.h
 * @author Krisna Pranav
 * @brief Traits
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "hashfunctions.h"

namespace Mods {

    /**
     * @brief GenericTraits
     * 
     * @tparam T 
     */
    template<typename T>
    struct GenericTraits {
        using PeekType = T;
        static constexpr bool is_trivial() { return false; }
        static bool equals(const T& a, const T& b) { return a == b; }
    };

    /**
     * @brief GenericTraits
     * 
     * @tparam T 
     */
    template<typename T>
    struct Traits : public GenericTraits<T> {
    };

    template<>
    struct Traits<int> : public GenericTraits<int> {
        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief hash
         * 
         * @param i 
         * @return unsigned 
         */
        static unsigned hash(int i) { 
            return int_hash(i); 
        }
    };

    template<>
    struct Traits<unsigned> : public GenericTraits<unsigned> {
        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief hash
         * 
         * @param u 
         * @return unsigned 
         */
        static unsigned hash(unsigned u) { 
            return int_hash(u); 
        }
    };

    template<>
    struct Traits<u8> : public GenericTraits<u8> {
        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief int_hash
         * 
         * @param u 
         * @return unsigned 
         */
        static unsigned hash(u8 u) { 
            return int_hash(u); 
        }
    };

    template<>
    struct Traits<u16> : public GenericTraits<u16> {
        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief hash
         * 
         * @param u 
         * @return unsigned 
         */
        static unsigned hash(u16 u) { 
            return int_hash(u); 
        }
    };

    template<>
    struct Traits<u64> : public GenericTraits<u64> {

        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief hash
         * 
         * @param u 
         * @return unsigned 
         */
        static unsigned hash(u64 u) { 
            return u64_hash(u); 
        }
    };

    /**
     * @brief Traits
     * 
     * @tparam  
     */
    template<>
    struct Traits<char> : public GenericTraits<char> {
        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief hash
         * 
         * @param c 
         * @return unsigned 
         */
        static unsigned hash(char c) { 
            return int_hash(c); 
        }
    };

    /**
     * @brief GenericTraits
     * 
     * @tparam T 
     */
    template<typename T>
    struct Traits<T*> : public GenericTraits<T*> {  
        
        /**
         * @brief hash
         * 
         * @param p 
         * @return unsigned 
         */
        static unsigned hash(const T* p) {
            return int_hash((unsigned)(__PTRDIFF_TYPE__)p);
        }

        /**
         * @brief is_trivial
         * 
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() { 
            return true; 
        }

        /**
         * @brief equals
         * 
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(const T* a, const T* b) { 
            return a == b; 
        }
    };

}

using Mods::GenericTraits;
using Mods::Traits;
