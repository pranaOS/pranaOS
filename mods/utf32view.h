/**
 * @file utf32view.h
 * @author Krisna Pranav
 * @brief Utf32View
 * @version 1.0
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "assertions.h"
#include "check.h"
#include "types.h"

namespace Mods 
{
    class Utf32View;

    class Utf32CodepointIterator 
    {
        friend class Utf32View;

    public:
        Utf32CodepointIterator() {}
        ~Utf32CodepointIterator() {}

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Utf32CodepointIterator& other) const
        {
            return m_ptr == other.m_ptr && m_length == other.m_length;
        }   

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Utf32CodepointIterator& other) const
        {
            return !(*this == other);
        }

        /**
         * @return Utf32CodepointIterator& 
         */
        Utf32CodepointIterator& operator++()
        {
            ASSERT(m_length > 0);
            m_ptr++;
            m_length--;
            return this;
        }

        /**
         * @param other 
         * @return ssize_t 
         */
        ssize_t operator-(const Utf32CodepointIterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

        /**
         * @return u32 
         */
        u32 operator*() const
        {
            ASSERT(m_length > 0);
            return *m_ptr;
        }

        /**
         * @return constexpr int 
         */
        constexpr int code_point_length_in_bytes() const
        {
            return sizeof(u32);
        }

        /**
         * @return true 
         * @return false 
         */
        bool done() const
        {
            return !m_length;
        }

    private:

        /**
         * @param ptr 
         * @param length 
         */
        Utf32CodepointIterator(const u32* ptr, size_t length)
            : m_ptr(ptr)
            , m_length((size_t)length)
        {}

        const u32* m_ptr { nullptr };

        ssize_t m_length { -1 };
    }; // class Utf32CodepointIterator
}