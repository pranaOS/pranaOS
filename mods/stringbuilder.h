/**
 * @file string_builder.h
 * @author Krisna Pranav
 * @brief StringBuilder
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/format.h>
#include <mods/forward.h>
#include <mods/stringview.h>
#include <stdarg.h>

namespace Mods
{
    class StringBuilder
    {
    public:
        using OutputType = String;

        /**
         * @brief Construct a new String Builder object
         * 
         * @param initial_capacity 
         */
        explicit StringBuilder(size_t initial_capacity = inline_capacity);

        /**
         * @brief Destroy the String Builder object
         * 
         */
        ~StringBuilder() = default;

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append(StringView);
    #ifndef KERNEL
        ErrorOr<void> try_append(Utf16View const&);
    #endif
        ErrorOr<void> try_append(Utf32View const&);

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append_code_point(u32);

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append(char);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         * @return ErrorOr<void> 
         */
        template <typename... Parameters>
        ErrorOr<void> try_appendff(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
        {
            VariadicFormatParams variadic_format_params{parameters...};
            return vformat(*this, fmtstr.view(), variadic_format_params);
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append(char const*, size_t);

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append_escaped_for_json(StringView);

        void append(StringView);
    #ifndef KERNEL
        void append(Utf16View const&);
    #endif
        void append(Utf32View const&);
        void append(char);
        void append_code_point(u32);
        void append(char const*, size_t);
        void appendvf(char const*, va_list);

        void append_as_lowercase(char);
        void append_escaped_for_json(StringView);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         */
        template <typename... Parameters>
        void appendff(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
        {
            VariadicFormatParams variadic_format_params{parameters...};
            MUST(vformat(*this, fmtstr.view(), variadic_format_params));
        }

    #ifndef KERNEL
        [[nodiscard]] String build() const;
        [[nodiscard]] String to_string() const;
    #endif
        [[nodiscard]] ByteBuffer to_byte_buffer() const;

        [[nodiscard]] StringView string_view() const;

        void clear();

        [[nodiscard]] size_t length() const
        {
            return m_buffer.size();
        }
        [[nodiscard]] bool is_empty() const
        {
            return m_buffer.is_empty();
        }
        
        /**
         * @param count 
         */
        void trim(size_t count)
        {
            m_buffer.resize(m_buffer.size() - count);
        }

        /**
         * @tparam SeparatorType 
         * @tparam CollectionType 
         * @param separator 
         * @param collection 
         * @param fmtstr 
         */
        template <class SeparatorType, class CollectionType>
        void join(SeparatorType const& separator, CollectionType const& collection, StringView fmtstr = "{}"sv)
        {
            bool first = true;
            for(auto& item : collection)
            {
                if(first)
                    first = false;
                else
                    append(separator);
                appendff(fmtstr, item);
            }
        }

    private:
        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> will_append(size_t);

        /**
         * @return u8* 
         */
        u8* data()
        {
            return m_buffer.data();
        }

        /**
         * @return u8 const* 
         */
        u8 const* data() const
        {
            return m_buffer.data();
        }

        static constexpr size_t inline_capacity = 256;
        Mods::Detail::ByteBuffer<inline_capacity> m_buffer;
    }; // class StringBuilder
} // namespace Mods

using Mods::StringBuilder;