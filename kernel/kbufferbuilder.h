/**
 * @file kbufferbuilder.h
 * @author Krisna Pranav 
 * @brief k buffer builder 
 * @version 6.0
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
    private:
        bool can_append(size_t) const;

        u8* insertion_ptr() 
        {
            return m_buffer.data();
        }

        KBuffer m_buffer;
        size_t m_size { 0 };
        
    }; // class KBufferBuidler

} // namespace Kernel