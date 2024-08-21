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


namespace Mods {

    template<typename T>
    class alignas(T) [[nodiscard]] Optional {
    public:
        Optional() {}

        /**
         * @brief Construct a new Optional object
         * 
         * @param value 
         */
        Optional(const T& value)
            : m_has_value(true)
        {
            new (&m_storage) T(value);
        }
        
        /**
         * @tparam U 
         * @param value 
         */
        template<typename U>
        Optional(const U& value)
            : m_has_value(true)
        {
            new (&m_storage) T(value);
        }

        /** 
         * @param value 
         */
        Optional(T && value)
            : m_has_value(true)
        {
            new (&m_storage) T(move(value));
        }

        /**
         * @param other 
         */
        Optional(Optional && other)
            : m_has_value(other.m_has_value)
        {
            if (other.has_value()) {
                new (&m_storage) T(other.release_value());
                other.m_has_value = false;
            }
        }

        /**
         * @param other 
         */
        Optional(const Optional& other)
            : m_has_value(other.m_has_value)
        {
            if (m_has_value) {
                new (&m_storage) T(other.value_without_consume_state());
            }
        }

        /**
         * @param other 
         * @return Optional& 
         */
        Optional& operator=(const Optional& other) {
            if (this != &other) {
                clear();
                m_has_value = other.m_has_value;
                if (m_has_value) {
                    new (&m_storage) T(other.value());
                }
            }
            return *this;
        }

        /**
         * @param other 
         * @return Optional& 
         */
        Optional& operator=(Optional&& other) {
            if (this != &other) {
                clear();
                m_has_value = other.m_has_value;
                if (other.has_value())
                    new (&m_storage) T(other.release_value());
            }
            return *this;
        }

        /**
         * @tparam O 
         * @param other 
         * @return true 
         * @return false 
         */
        template<typename O>
        bool operator==(const Optional<O>& other) const {
            return has_value() == other.has_value() && (!has_value() || value() == other.value());
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~Optional() {
            clear();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void clear() {
            if (m_has_value) {
                value().~T();
                m_has_value = false;
            }
        }

        /**
         * @brief emplace
         * 
         * @tparam Parameters 
         * @param parameters 
         * @return ALWAYS_INLINE 
         */
        template<typename... Parameters>
        ALWAYS_INLINE void emplace(Parameters && ... parameters) {
            clear();
            m_has_value = true;
            new (&m_storage) T(forward<Parameters>(parameters)...);
        }

        /**
         * @brief has_value
         * 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool has_value() const { 
            return m_has_value; 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& value() {
            ASSERT(m_has_value);
            return *reinterpret_cast<T*>(&m_storage);
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& value() const {
            return value_without_consume_state();
        }

        /**
         * @return T 
         */
        T release_value() {
            ASSERT(m_has_value);
            T released_value = move(value());
            value().~T();
            m_has_value = false;
            return released_value;
        }

        /**
         * @param fallback 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T value_or(const T& fallback) const {
            if (m_has_value)
                return value();
            return fallback;
        }

    private:
    
        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& value_without_consume_state() const {
            ASSERT(m_has_value);
            return *reinterpret_cast<const T*>(&m_storage);
        }

        u8 m_storage[sizeof(T)] { 0 };
        bool m_has_value { false };
    };

}

// using mods
using Mods::Optional;
