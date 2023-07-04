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

        explicit StringBuilder(size_t initial_capacity = inline_capacity);
        ~StringBuilder() {}

        void append(const StringView&);

        void append(const Utf32View);

        void append(char);

        void append_code_point(u32);
        void append(const char*, size_t);

        /**
         * @param ... 
         */
        void appendf(const char*, ...);
        
        /**
         * @return void
         */
        void appendvf(const char*, va_list);

        void append_escaped_for_json(const StringView&);

        template<typename... Parameters>
        void appendff(StringView fmtstr, const Parameters&... parameters) {
            vformat(*this, fmtstr, VariadicFormatParams { parameters... });
        }

        String build() const;
        String to_string() const;

        size_t length() const {
            return m_length;
        }

    private:
        void will_append(size_t);

        u8* data() {
            return m_buffer.is_null() ? m_inline_buffer : m_buffer.data();
        };

        const u8* data() const {
            return m_buffer.is_null() ? m_inline_buffer : m_buffer.data();
        };

        static constexpr size_t inline_capacity = 128;
        u8 m_inline_buffer[inline_capacity];
        ByteBuffer m_buffer;
        size_t m_length { 0 };
    };
}
