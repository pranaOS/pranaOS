/**
 * @file string_builder.h
 * @author Krisna Pranav
 * @brief StringBuilder
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdarg.h>
#include "byte_buffer.h"
#include "format.h"
#include "forward.h"
#include "string_view.h"

namespace Mods {

    class StringBuilder {
    public:
        using OutputType = String;

        /**
         * @brief Construct a new String Builder object
         * 
         * @param initial_capacity 
         */
        explicit StringBuilder(size_t initial_capacity = inline_capacity);

        ~StringBuilder() {}

        void append(const StringView&);
        void append(const Utf32View&);
        void append(char);
        void append_code_point(u32);

        /**
         * @param size_t
         */
        void append(const char*, size_t);

        /**
         * @param ... 
         */
        void appendf(const char*, ...);

        /**
         * @param va_list
         */
        void appendvf(const char*, va_list);

        void append_escaped_for_json(const StringView&);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         */
        template<typename... Parameters>
        void appendff(StringView fmtstr, const Parameters&... parameters) {
            vformat(*this, fmtstr, VariadicFormatParams { parameters... });
        }

        String build() const;
        String to_string() const;

        /**
         * @return ByteBuffer 
         */
        ByteBuffer to_byte_buffer() const;

        /**
         * @return StringView 
         */
        StringView string_view() const;

        // clear(destroy)
        void clear();

        /**
         * @return size_t 
         */
        size_t length() const { 
            return m_length; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return m_length == 0; 
        }

        /** 
         * @param count 
         */
        void trim(size_t count) { 
            m_length -= count; 
        }

        /**
         * @tparam SeparatorType 
         * @tparam CollectionType 
         * @param separator 
         * @param collection 
         */
        template<class SeparatorType, class CollectionType>
        void join(const SeparatorType& separator, const CollectionType& collection) {
            bool first = true;
            for (auto& item : collection) {
                if (first)
                    first = false;
                else
                    append(separator);
                append(item);
            }
        }

    private:
        void will_append(size_t);

        /**
         * @return u8* 
         */
        u8* data() { 
            return m_buffer.is_null() ? m_inline_buffer : m_buffer.data(); 
        }

        /**
         * @return const u8* 
         */
        const u8* data() const { 
            return m_buffer.is_null() ? m_inline_buffer : m_buffer.data(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool using_inline_buffer() const { 
            return m_buffer.is_null(); 
        }

        // capacity
        static constexpr size_t inline_capacity = 128;

        // inline buffers
        u8 m_inline_buffer[inline_capacity];

        // buffer & length
        ByteBuffer m_buffer;
        size_t m_length { 0 };
    };

}

using Mods::StringBuilder;
