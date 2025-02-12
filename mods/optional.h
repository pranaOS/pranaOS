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

#include <mods/assertions.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <mods/kmalloc.h>

namespace Mods
{
    /**
     * @tparam typename 
     */
    template <typename>
    class Optional;

    /**
     * @tparam T 
     */
    template <typename T>
        requires(!IsLvalueReference<T>)
    class [[nodiscard]] Optional<T>
    {
        /**
         * @tparam U 
         */
        template <typename U>
        friend class Optional;

        static_assert(!IsLvalueReference<T> && !IsRvalueReference<T>);

    public:
        using ValueType = T;

        ALWAYS_INLINE Optional() = default;

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
        Optional(Optional const& other)
            requires(!IsCopyConstructible<T>)
        = delete;
        Optional(Optional const& other) = default;

        Optional(Optional&& other)
            requires(!IsMoveConstructible<T>)
        = delete;

        Optional& operator=(Optional const&)
            requires(!IsCopyConstructible<T> || !IsDestructible<T>)
        = delete;
        Optional& operator=(Optional const&) = default;

        Optional& operator=(Optional&& other)
            requires(!IsMoveConstructible<T> || !IsDestructible<T>)
        = delete;

        ~Optional()
            requires(!IsDestructible<T>)
        = delete;
        ~Optional() = default;
    #endif

        ALWAYS_INLINE Optional(Optional const& other)
    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!IsTriviallyCopyConstructible<T>)
    #endif
            : m_has_value(other.m_has_value)
        {
            if(other.has_value())
                new(&m_storage) T(other.value());
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional(Optional&& other)
            : m_has_value(other.m_has_value)
        {
            if(other.has_value())
                new(&m_storage) T(other.release_value());
        }

        /**
         * @tparam U 
         */
        template <typename U>
            requires(IsConstructible<T, U const&> && !IsSpecializationOf<T, Optional> && !IsSpecializationOf<U, Optional>)
        ALWAYS_INLINE explicit Optional(Optional<U> const& other)
            : m_has_value(other.m_has_value)
        {
            if(other.has_value())
                new(&m_storage) T(other.value());
        }

        /**
         * @tparam U 
         */
        template <typename U>
            requires(IsConstructible<T, U &&> && !IsSpecializationOf<T, Optional> && !IsSpecializationOf<U, Optional>)
        ALWAYS_INLINE explicit Optional(Optional<U>&& other)
            : m_has_value(other.m_has_value)
        {
            if(other.has_value())
                new(&m_storage) T(other.release_value());
        }

        /**
         * @tparam U 
         */
        template <typename U = T>
        ALWAYS_INLINE explicit(!IsConvertible<U&&, T>) Optional(U&& value)
            requires(!IsSame<RemoveCVReference<U>, Optional<T>> && IsConstructible<T, U &&>)
            : m_has_value(true)
        {
            new(&m_storage) T(forward<U>(value));
        }

        ALWAYS_INLINE Optional& operator=(Optional const& other)
    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!IsTriviallyCopyConstructible<T> || !IsTriviallyDestructible<T>)
    #endif
        {
            if(this != &other)
            {
                clear();
                m_has_value = other.m_has_value;
                if(other.has_value())
                {
                    new(&m_storage) T(other.value());
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
            if(this != &other)
            {
                clear();
                m_has_value = other.m_has_value;
                if(other.has_value())
                {
                    new(&m_storage) T(other.release_value());
                }
            }
            return *this;
        }

        /**
         * @tparam O 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template <typename O>
        ALWAYS_INLINE bool operator==(Optional<O> const& other) const
        {
            return has_value() == other.has_value() && (!has_value() || value() == other.value());
        }

        /**
         * @tparam O 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template <typename O>
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
            if(m_has_value)
            {
                value().~T();
                m_has_value = false;
            }
        }

        /**
         * @tparam Parameters 
         * @param parameters 
         * @return ALWAYS_INLINE 
         */
        template <typename... Parameters>
        ALWAYS_INLINE void emplace(Parameters&&... parameters)
        {
            clear();
            m_has_value = true;
            new(&m_storage) T(forward<Parameters>(parameters)...);
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
            if(m_has_value)
                return value();
            return fallback;
        }

        /**
         * @param fallback 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE T value_or(T&& fallback) &&
        {
            if(m_has_value)
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
        bool m_has_value{false};
    }; // class [[nodiscard]] Optional<T>

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template <typename T>
        requires(IsLvalueReference<T>)
    class [[nodiscard]] Optional<T>
    {
        /**
         * @tparam typename 
         */
        template <typename>
        friend class Optional;

        /**
         * @tparam U 
         */
        template <typename U>
        constexpr static bool CanBePlacedInOptional = IsSame<RemoveReference<T>, RemoveReference<AddConstToReferencedType<U>>> && (IsBaseOf<RemoveCVReference<T>, RemoveCVReference<U>> || IsSame<RemoveCVReference<T>, RemoveCVReference<U>>);

    public:
        using ValueType = T;

        ALWAYS_INLINE Optional() = default;

        /**
         * @tparam U 
         */
        template <typename U = T>
        ALWAYS_INLINE Optional(U& value)
            requires(CanBePlacedInOptional<U&>)
            : m_pointer(&value)
        {
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional(RemoveReference<T>& value)
            : m_pointer(&value)
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional(Optional const& other)
            : m_pointer(other.m_pointer)
        {
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional(Optional&& other)
            : m_pointer(other.m_pointer)
        {
            other.m_pointer = nullptr;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE Optional(Optional<U> const& other)
            requires(CanBePlacedInOptional<U>)
            : m_pointer(other.m_pointer)
        {
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE Optional(Optional<U>&& other)
            requires(CanBePlacedInOptional<U>)
            : m_pointer(other.m_pointer)
        {
            other.m_pointer = nullptr;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE Optional& operator=(Optional const& other)
        {
            m_pointer = other.m_pointer;
            return *this;
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE Optional& operator=(Optional&& other)
        {
            m_pointer = other.m_pointer;
            other.m_pointer = nullptr;
            return *this;
        }   

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE Optional& operator=(Optional<U> const& other)
            requires(CanBePlacedInOptional<U>)
        {
            m_pointer = other.m_pointer;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE Optional& operator=(Optional<U>&& other)
            requires(CanBePlacedInOptional<U>)
        {
            m_pointer = other.m_pointer;
            other.m_pointer = nullptr;
            return *this;
        }

        /**
         * @tparam U 
         */
        template <typename U>
        ALWAYS_INLINE Optional& operator=(U&& value)
            requires(CanBePlacedInOptional<U> && IsLvalueReference<U>)
        {
            m_pointer = &value;
            return *this;
        }

        ALWAYS_INLINE void clear()
        {
            m_pointer = nullptr;
        }

        [[nodiscard]] ALWAYS_INLINE bool has_value() const
        {
            return m_pointer != nullptr;
        }

        [[nodiscard]] ALWAYS_INLINE T value()
        {
            VERIFY(m_pointer);
            return *m_pointer;
        }

        [[nodiscard]] ALWAYS_INLINE AddConstToReferencedType<T> value() const
        {
            VERIFY(m_pointer);
            return *m_pointer;
        }

        /**
         * @tparam U 
         */
        template <typename U>
            requires(IsBaseOf<RemoveCVReference<T>, U>)
        [[nodiscard]] ALWAYS_INLINE AddConstToReferencedType<T> value_or(U& fallback) const
        {
            if(m_pointer)
                return value();
            return fallback;
        }

        /**
         * @param fallback 
         * @return ALWAYS_INLINE 
         */
        [[nodiscard]] ALWAYS_INLINE RemoveCVReference<T> value_or(RemoveCVReference<T> fallback) const
        {
            if(m_pointer)
                return value();
            return fallback;
        }

        [[nodiscard]] ALWAYS_INLINE T release_value()
        {
            return *exchange(m_pointer, nullptr);
        }

        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template <typename U>
        ALWAYS_INLINE bool operator==(Optional<U> const& other) const
        {
            return has_value() == other.has_value() && (!has_value() || value() == other.value());
        }
        
        /**
         * @tparam U 
         * @param other 
         * @return ALWAYS_INLINE 
         */
        template <typename U>
        ALWAYS_INLINE bool operator==(U const& other) const
        {
            return has_value() && value() == other;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE AddConstToReferencedType<T> operator*() const
        {
            return value();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T operator*()
        {
            return value();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RawPtr<AddConst<RemoveReference<T>>> operator->() const
        {
            return &value();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE RawPtr<RemoveReference<T>> operator->()
        {
            return &value();
        }

        /**
         * @return Optional<RemoveCVReference<T>> 
         */
        ALWAYS_INLINE operator Optional<RemoveCVReference<T>>() const
        {
            if(has_value())
                return Optional<RemoveCVReference<T>>(value());
            return {};
        }

    private:
        RemoveReference<T>* m_pointer{nullptr};
    }; // class [[nodiscard]] Optional<T>
} // namespace Mods

using Mods::Optional;