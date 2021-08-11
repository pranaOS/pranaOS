/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/ByteBuffer.h>
#include <base/Format.h>
#include <base/Forward.h>
#include <base/StringView.h>
#include <stdarg.h>

namespace Base {

class StringBuilder {
public:
    using OutputType = String;

    explicit StringBuilder(size_t initial_capacity = inline_capacity);
    ~StringBuilder() = default;

    void append(const StringView&);
    void append(const Utf32View&);
    void append(char);
    void append_code_point(u32);
    void append(const char*, size_t);
    void appendvf(const char*, va_list);

    void append_as_lowercase(char);
    void append_escaped_for_json(const StringView&);

    template<typename... Parameters>
    void appendff(CheckedFormatString<Parameters...>&& fmtstr, const Parameters&... parameters)
    {
        vformat(*this, fmtstr.view(), VariadicFormatParams { parameters... });
    }

    [[nodiscard]] String build() const;
    [[nodiscard]] String to_string() const;
    [[nodiscard]] ByteBuffer to_byte_buffer() const;

    [[nodiscard]] StringView string_view() const;
    void clear();

    [[nodiscard]] size_t length() const { return m_buffer.size(); }
    [[nodiscard]] bool is_empty() const { return m_buffer.is_empty(); }
    void trim(size_t count) { m_buffer.resize(m_buffer.size() - count); }

    template<class SeparatorType, class CollectionType>
    void join(const SeparatorType& separator, const CollectionType& collection)
    {
        bool first = true;
        for (auto& item : collection) {
            if (first)
                first = false;
            else
                append(separator);
            appendff("{}", item);
        }
    }

private:
    void will_append(size_t);
    u8* data() { return m_buffer.data(); }
    const u8* data() const { return m_buffer.data(); }

    static constexpr size_t inline_capacity = 128;
    Base::Detail::ByteBuffer<inline_capacity> m_buffer;
};

}

using Base::StringBuilder;
