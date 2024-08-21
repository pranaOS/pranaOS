/**
 * @file kbufferbuilder.h
 * @author Krisna Pranav 
 * @brief k buffer builder 
 * @version 6.0
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <kernel/kbuffer.h>
#include <stdarg.h>

namespace Kernel 
{

    class KBufferBuilder 
    {
    public:
        using OutputType = KBuffer;

        /// @brief Construct a new KBufferBuilder object
        explicit KBufferBuilder();

        /// @brief Destroy the KBufferBuilder object
        ~KBufferBuilder() { }

        void append(const StringView&);
        void append(char);
        
        void append(const char*, int);
        void appendf(const char*, ...);

        void appendvf(const char*, va_list);

        void append_escaped_for_json(const StringView&);

        /**
         * @tparam Parameters 
         * @param fmtstr 
         * @param parameters 
         */
        template<typename... Parameters>
        void appendff(StringView fmtstr, const Parameters&... parameters)
        {
            append(String::formatted(fmtstr, parameters...));
        }

        KBuffer build();

    private:
        bool can_append(size_t) const;

        /**
         * @return u8* 
         */
        u8* insertion_ptr() 
        { 
            return m_buffer.data() + m_size; 
        }

        KBuffer m_buffer;

        size_t m_size { 0 };
    }; // class KBufferBuilder

} // namespace Kernel