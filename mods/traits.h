/**
 * @file traits.h
 * @author Krisna Pranav
 * @brief Traits
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "hashfunctions.h"
#include "forward.h"


namespace Mods {

    template<typename T>
    struct GenericTraits {
        using PeekType = T;
        static constexpr bool is_trivial() {
            return false;
        }

        static bool equals(const T& a, const T& b) {
            return a == b; 
        }
    };

    template<typename T>
    struct Traits : public GenericTraits<T> {
    }

    template<>
    struct Traits<int>: public GenericTraits<int> {
        static constexpr bool is_trivial() {
            return true;
        }
        
        static unsigned hash(int i) {
            return int_hash(i);
        }
    }

    template<>
    struct Traits<unsigned>: public GenericTraits<unsigned> {
        static constexpr bool is_trivial() {
            return true;
        }

        static unsigned hash(unsigned u) {
            return int_hash(u);
        }
    }

    template<typename t>
    struct Traits<T*>: public GenericTraits<T*> {
        static unsigned hash(const T* p) {
            return int_hash((unsigned)(__PTRDIFF_TYPE__)p);
        }

        static constexpr bool is_trivial() {
            return true;
        }
        
        static bool equals(const T* a, const T* b) {
            return a == b;
        }
    };
}

using Mods::GenericTraits;
using Mods::Traits;