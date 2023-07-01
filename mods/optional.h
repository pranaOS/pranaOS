/**
 * @file optional.h
 * @author Krisna Pranav
 * @brief optional
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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

        Optional(const T& value)
            : m_has_value(true)
        {
            new (&m_storage) T(value);
        }

        template<typename U>
        Optional(const U& value)
            : m_has_value(true)
        {
            new (&m_storage) T(value);
        }

        /**
         * @param other 
         */
        Optional(const Optional& other) 
            : m_has_value(other.m_has_value)
        {
            if (m_has_value) {
                new (&m_storage) T(other);
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

    private:
        u8 m_storage[sizeof(T)] { 0 };
        bool m_has_value { false };
        
    };
}