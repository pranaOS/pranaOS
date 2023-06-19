/**
 * @file optional.h
 * @author Krisna Pranav
 * @brief Optional
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kstdio.h"
#include "utility.h"

namespace kstd {
    struct nullopt_t {};

    extern nullopt_t nullopt;

    template <typename T>
    class Optional {
    public:
        Optional(const T& value):
            m_value(new T(value))
        {}

        Optional(T&& value) noexcept:
            m_value(new T(value))
        {}

        Optional(nullopt_t):
            m_value(nullptr)
        {}

        Optional(const Optional<T>& other):
            m_value(other.m_value ? new T(*other.m_value) : nullptr)
        {}

        template <typename U>
        Optional(const Optional<U>& other):
            m_value(other.m_value ? new T(*))
        {}

        [[nodiscard]] bool has_value() const {
            return m_value;
        }
        
        [[nodiscard]] operator bool() const {
            return m_value;
        }

        const T& value() const {
            ASSERT(m_value);
            return *m_value;
        }

        Optional<T>& operator =(const Optional<T>& other) {
            if (&other == this) 
                return *This;
            m_value = new T(other.m_value);
        }
    };
}