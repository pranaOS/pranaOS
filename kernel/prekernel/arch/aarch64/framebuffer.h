/**
 * @file framebuffer.cpp
 * @author Krisna Pranav
 * @brief Frame Buffer
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
    class Framebuffer 
    {
    public:
        enum class PixelOrder 
        {
            RGB,
            BGR,
        }; // enum class PixelOrder

        static Framebuffer& the();

        bool initialized() const 
        { 
            return m_initialized; 
        }

        /**
         * @return u16 
         */
        u16 width() const 
        { 
            return m_width; 
        }

        u16 height() const 
        { 
            return m_height; 
        }

        u8 depth() const 
        { 
            return m_depth; 
        }
        
        /**
         * @return u8* 
         */
        u8* gpu_buffer() const 
        { 
            return m_gpu_buffer; 
        }
        
        /**
         * @return u32 
         */
        u32 buffer_size() const 
        { 
            return m_buffer_size; 
        }

        u32 pitch() const 
        { 
            return m_pitch; 
        }

        /**
         * @return PixelOrder 
         */
        PixelOrder pixel_order() 
        { 
            return m_pixel_order; 
        }

    private:
        u16 m_width;
        u16 m_height;
        u8 m_depth;
        u8* m_gpu_buffer;
        u32 m_buffer_size;
        u32 m_pitch;
        bool m_initialized;
        PixelOrder m_pixel_order;

        Framebuffer();
    }; // class Framebuffer 
} // namespace Prekernel
