/**
 * @file framebuffer.cpp
 * @author Krisna Pranav
 * @brief Frame Buffer
 * @version 6.0
 * @date 2024-10-23
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#include <kernel/prekernel/arch/aarch64/framebuffer.h>
#include <kernel/prekernel/arch/aarch64/framebuffermailboxmessage.h>
#include <kernel/prekernel/arch/aarch64/utils.h>

namespace Prekernel
{
    /**
     * @brief Construct a new Framebuffer::Framebuffer object
     * 
     */
    Framebuffer::Framebuffer()
    {
        m_width = 1280;
        m_height = 720;
        m_depth = 32;
        m_initialized = false;

        struct __attribute__((aligned(16)))
        {
            Mailbox::MessageHeader header;
            FramebufferSetPhysicalSizeMboxMessage set_physical_size;
            FramebufferSetVirtualSizeMboxMessage set_virtual_size;
            FramebufferSetVirtualOffsetMboxMessage set_virtual_offset;
            FramebufferSetDepthMboxMessage set_depth;
            FramebufferSetPixelOrderMboxMessage set_pixel_order;
            FramebufferAllocateBufferMboxMessage allocate_buffer;
            FramebufferGetPithMboxMessage get_pitch;
            Mailbox::MessageTail tail;
        } message_queue;

        message_queue.header.set_queue_size(sizeof(message_queue));
        message_queue.set_physical_size.width = m_width;
        message_queue.set_physical_size.height = m_height;
        message_queue.set_virtual_size.width = m_width;
        message_queue.set_virtual_size.height = m_height;

        message_queue.set_depth.depth_bits = 32;
        message_queue.set_pixel_order.pixel_order = FramebufferSetPixelOrderMboxMessage::PixelOrder::RGB;
        message_queue.allocate_buffer.alignment = 4096;

        if(!Mailbox::the().send_queue(&message_queue, sizeof(message_queue)))
        {
            warnln("Framebuffer: Mailbox send failed.");
            return;
        }

        if(message_queue.set_physical_size.width != m_width || message_queue.set_physical_size.height != m_height)
        {
            warnln("Framebuffer(): Setting physical dimension failed.");
            return;
        }

        if(message_queue.set_virtual_size.width != m_width || message_queue.set_virtual_size.height != m_height)
        {
            warnln("Framebuffer(): Setting virtual dimension failed.");
            return;
        }

        if(message_queue.set_virtual_offset.x != 0 || message_queue.set_virtual_offset.y != 0)
        {
            warnln("Framebuffer(): Setting virtual offset failed.");
            return;
        }

        if(message_queue.set_depth.depth_bits != m_depth)
        {
            warnln("Framebuffer(): Setting depth failed.");
            return;
        }

        if(message_queue.allocate_buffer.size == 0 || message_queue.allocate_buffer.address == 0)
        {
            warnln("Framebuffer(): Allocating buffer failed.");
            return;
        }

        if(message_queue.get_pitch.pitch == 0)
        {
            warnln("Framebuffer(): Retrieving pitch failed.");
            return;
        }

        m_gpu_buffer = reinterpret_cast<u8*>(message_queue.allocate_buffer.address & 0x3FFFFFFF);

        m_buffer_size = message_queue.allocate_buffer.size;
        m_pitch = message_queue.get_pitch.pitch;

        switch(message_queue.set_pixel_order.pixel_order)
        {
        case FramebufferSetPixelOrderMboxMessage::PixelOrder::RGB:
            m_pixel_order = PixelOrder::RGB;
            break;
        case FramebufferSetPixelOrderMboxMessage::PixelOrder::BGR:
            m_pixel_order = PixelOrder::BGR;
            break;
        default:
            warnln("Framebuffer: Unsupported pixel order reported by GPU.");
            m_pixel_order = PixelOrder::RGB;
            break;
        }

        dbgln("Initialized framebuffer: 1280 x 720 @ 32 bits");
        m_initialized = true;
    }

    /**
     * @return Framebuffer& 
     */
    Framebuffer& Framebuffer::the()
    {
        static Framebuffer instance;
        return instance;
    }
} // namespace Prekernel