/**
 * @file optional.h
 * @author Krisna Pranav
 * @brief optional
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kmalloc.h"
#include "platform.h"
#include "stdlibextra.h"
#include "types.h"
#include "assertions.h"

namespace Mods 
{
    /**
     * @tparam T 
     */
    template<typename T>
    class [[nodiscard]] Optional 
    {
        template<typename U>
        friend class Optional;

    public:
        using ValueType = T;

        ALWAYS_INLINE Optional() = default;

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
        
        Optional(Optional const& other) requires(!IsCopyConstructible<T>) = delete;
        Optional(Optional const& other) = default;

        Optional(Optional&& other) requires(!IsMoveConstructible<T>) = delete;

        Optional& operator=(Optional const&) requires(!IsCopyConstructible<T> || !IsDestructible<T>) = delete;
        Optional& operator=(Optional const&) = default;

        Optional& operator=(Optional&& other) requires(!IsMoveConstructible<T> || !IsDestructible<T>) = delete;

        ~Optional() requires(!IsDestructible<T>) = delete;

        ~Optional() = default;
    #endif

        ALWAYS_INLINE Optional(Optional const& other)
    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!IsTriviallyCopyConstructible<T>)
    #endif
            : m_has_value(other.m_has_value)
        {
            if (other.has_value())
                new (&m_storage) T(other.value());
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional(Optional&& other)
            : m_has_value(other.m_has_value)
        {
            if (other.has_value())
                new (&m_storage) T(other.release_value());
        }

        /**
         * @tparam U 
         */
        template<typename U>
        requires(IsConstructible<T, U const&> && !IsSpecializationOf<T, Optional> && !IsSpecializationOf<U, Optional>) ALWAYS_INLINE explicit Optional(Optional<U> const& other)
            : m_has_value(other.m_has_value)
        {
            if (other.has_value())
                new (&m_storage) T(other.value());
        }

        /**
         * @tparam U 
         */
        template<typename U>
        requires(IsConstructible<T, U&&> && !IsSpecializationOf<T, Optional> && !IsSpecializationOf<U, Optional>) ALWAYS_INLINE explicit Optional(Optional<U>&& other)
            : m_has_value(other.m_has_value)
        {
            if (other.has_value())
                new (&m_storage) T(other.release_value());
        }

        /**
         * @tparam U 
         */
        template<typename U = T>
        ALWAYS_INLINE explicit(!IsConvertible<U&&, T>) Optional(U&& value) requires(!IsSame<RemoveCVReference<U>, Optional<T>> && IsConstructible<T, U&&>)
            : m_has_value(true)
        {
            new (&m_storage) T(forward<U>(value));
        }

        ALWAYS_INLINE Optional& operator=(Optional const& other)
    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!IsTriviallyCopyConstructible<T> || !IsTriviallyDestructible<T>)
    #endif
        {
            if (this != &other) {
                clear();
                m_has_value = other.m_has_value;
                if (other.has_value()) {
                    new (&m_storage) T(other.value());
                }
            }
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE Optional& operator=(Optional&& other)
        {
            if (this != &other) {
                clear();
                m_has_value = other.m_has_value;
                if (other.has_value()) {
                    new (&m_storage) T(other.release_value());
                }
            }
            return *this;
        }

        /**
         * @tparam O 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename O>
        ALWAYS_INLINE bool operator==(Optional<O> const& other) const
        {
            return has_value() == other.has_value() && (!has_value() || value() == other.value());
        }

        /**
         * @tparam O 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template<typename O>
        ALWAYS_INLINE bool operator==(O const& other) const
        {
            return has_value() && value() == other;
        }

        ALWAYS_INLINE ~Optional()
    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!IsTriviallyDestructible<T>)
    #endif
        {
            clear();
        }

        ALWAYS_INLINE void clear()
        {
            if (m_has_value) {
                value().~T();
                m_has_value = false;
            }
        }

        /**
         * @tparam Parameters 
         * @param parameters 
         * @return ALWAYS_INLINE 
         */
        template<typename... Parameters>
        ALWAYS_INLINE void emplace(Parameters&&... parameters)
        {
            clear();
            m_has_value = true;
            new (&m_storage) T(forward<Parameters>(parameters)...);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE bool has_value() const 
        { 
            return m_has_value; 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        [[nodiscard]] ALWAYS_INLINE T& value() &
        {
            VERIFY(m_has_value);
            return *__builtin_launder(reinterpret_cast<T*>(&m_storage));
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        [[nodiscard]] ALWAYS_INLINE T const& value() const&
        {
            VERIFY(m_has_value);
            return *__builtin_launder(reinterpret_cast<T const*>(&m_storage));
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE T value() &&
        {
            return release_value();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE T release_value()
        {
            VERIFY(m_has_value);
            T released_value = move(value());
            value().~T();
            m_has_value = false;
            return released_value;
        }

        /**
         * @param fallback 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE T value_or(T const& fallback) const&
        {
            if (m_has_value)
                return value();
            return fallback;
        }

        /**
         * @param fallback 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE T value_or(T&& fallback) &&
        {
            if (m_has_value)
                return move(value());
            return move(fallback);
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE T const& operator*() const 
        { 
            return value(); 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator*() 
        { 
            return value(); 
        }

        /**
         * @return ALWAYS_INLINE const* 
         */
        ALWAYS_INLINE T const* operator->() const 
        { 
            return &value(); 
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE T* operator->() 
        { 
            return &value(); 
        }

    private:
        alignas(T) u8 m_storage[sizeof(T)];

        bool m_has_value { false };
    }; // class Optional
} // namespace Mods

using Mods::Optional;
