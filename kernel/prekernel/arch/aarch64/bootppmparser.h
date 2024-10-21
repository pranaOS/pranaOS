/**
 * @file bootppmparser.h
 * @author Krisna Pranav
 * @brief Boot PPM Parser
 * @version 6.0
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Prekernel 
{
    class BootPPMParser 
    {
    public:
        struct 
        {
            u32 width = 0;
            u32 height = 0;
            u8 const* pixel_data = nullptr;
        } image;

        /**
         * @brief Construct a new Boot P P M Parser object
         * 
         * @param buffer 
         * @param buffer_size 
         */
        BootPPMParser(u8 const* buffer, u32 buffer_size);

        bool parse();

    private:
        char const* m_cursor;
        char const* m_buffer_end;

        bool check_position() const;
        bool parse_magic();
        bool parse_new_line();
        bool parse_comment();

        /**
         * 
         * @param value 
         * @return true 
         * @return false 
         */
        bool parse_integer(u32& value);
    }; // class BootPPMParser

} // namespace Prekernel