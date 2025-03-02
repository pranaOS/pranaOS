/**
 * @file traits.h
 * @author Krisna Pranav
 * @brief Traits
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bitcast.h>
#include <mods/concept.h>
#include <mods/forward.h>
#include <mods/hashfunctions.h>
#include <mods/stringhash.h>
#include <string.h>

namespace Mods
{   
    /**
     * @tparam T 
     */
    template <typename T>
    struct GenericTraits
    {
        using PeekType = T&;
        using ConstPeekType = T const&;

        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial()
        {
            return false;
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static constexpr bool equals(const T& a, const T& b)
        {
            return a == b;
        }

        /**
         * @tparam U 
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        template <Concepts::HashCompatible<T> U>
        static bool equals(U const& a, T const& b)
        {
            return a == b;
        }
    }; // struct GenericTraits

    /**
     * @tparam T 
     */
    template <typename T>
    struct Traits : public GenericTraits<T>
    {
    };

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template <typename T>
        requires(IsIntegral<T>)
    struct Traits<T> : public GenericTraits<T>
    {
        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial()
        {
            return true;
        }

        /**
         * @param value 
         * @return constexpr unsigned 
         */
        static constexpr unsigned hash(T value)
        {
            if constexpr(sizeof(T) < 8)
                return int_hash(value);
            else
                return u64_hash(value);
        }
    }; // struct Traits<T> : public GenericTraits<T>

    #ifndef KERNEL
    template <typename T>
        requires(IsFloatingPoint<T>)
    struct Traits<T> : public GenericTraits<T>
    {
        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial()
        {
            return true;
        }

        /**
         * @param value 
         * @return constexpr unsigned 
         */
        static constexpr unsigned hash(T value)
        {
            if constexpr(sizeof(T) < 8)
                return int_hash(bit_cast<u32>(value));
            else
                return u64_hash(bit_cast<u64>(value));
        }
    };
    #endif

    /**
     * @tparam T 
     */
    template <typename T>
        requires(IsPointer<T> && !Detail::IsPointerOfType<char, T>)
    struct Traits<T> : public GenericTraits<T>
    {
        /**
         * @param p 
         * @return unsigned 
         */
        static unsigned hash(T p)
        {
            return ptr_hash((FlatPtr)p);
        }
        static constexpr bool is_trivial()
        {
            return true;
        }
    }; // struct Traits<T> : public GenericTraits<T>

    /**
     * @tparam T 
     */
    template <Enum T>
    struct Traits<T> : public GenericTraits<T>
    {
        /**
         * @param value 
         * @return unsigned 
         */
        static unsigned hash(T value)
        {
            return Traits<UnderlyingType<T>>::hash(to_underlying(value));
        }

        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial()
        {
            return Traits<UnderlyingType<T>>::is_trivial();
        }
    }; // struct Traits<T> : public GenericTraits<T>

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template <typename T>
        requires(Detail::IsPointerOfType<char, T>)
    struct Traits<T> : public GenericTraits<T>
    {
        /**
         * @param value 
         * @return unsigned 
         */
        static unsigned hash(T const value)
        {
            return string_hash(value, strlen(value));
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static constexpr bool equals(T const a, T const b)
        {
            return strcmp(a, b);
        }

        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial()
        {
            return true;
        }
    }; // struct Traits<T> : public GenericTraits<T>
} // namespace Mods

using Mods::GenericTraits;
using Mods::Traits;