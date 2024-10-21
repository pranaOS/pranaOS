/**
 * @file framebuffermailboxmessage.h
 * @author Krisna Pranav
 * @brief Frame Buffer MailBox Message
 * @version 6.0
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/prekernel/arch/aarch64/mailbox.h>

namespace Prekernel 
{

    class FramebufferSetPhysicalSizeMboxMessage : public Mailbox::Message 
    {
    public:
        u32 width;
        u32 height;

        /**
         * @brief Construct a new Framebuffer Set Physical Size Mbox Message object
         * 
         */
        FramebufferSetPhysicalSizeMboxMessage()
            : Mailbox::Message(0x48003, 8)
        {
            width = 0;
            height = 0;
        }
    }; // class FramebufferSetPhysicalSizeMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferSetPhysicalSizeMboxMessage) == 20);

    class FramebufferSetVirtualSizeMboxMessage : public Mailbox::Message 
    {
    public:
        u32 width;
        u32 height;

        /**
         * @brief Construct a new Framebuffer Set Virtual Size Mbox Message object
         * 
         */
        FramebufferSetVirtualSizeMboxMessage()
            : Mailbox::Message(0x48004, 8)
        {
            width = 0;
            height = 0;
        }
    }; // class FramebufferSetVirtualSizeMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferSetVirtualSizeMboxMessage) == 20);

    class FramebufferSetVirtualOffsetMboxMessage : public Mailbox::Message 
    {
    public:
        u32 x;
        u32 y;

        /**
         * @brief Construct a new Framebuffer Set Virtual Offset Mbox Message object
         * 
         */
        FramebufferSetVirtualOffsetMboxMessage()
            : Mailbox::Message(0x48009, 8)
        {
            x = 0;
            y = 0;
        }
    }; // class FramebufferSetVirtualOffsetMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferSetVirtualOffsetMboxMessage) == 20);

    class FramebufferSetDepthMboxMessage : public Mailbox::Message 
    {
    public:
        u32 depth_bits;

        /**
         * @brief Construct a new Framebuffer Set Depth Mbox Message object
         * 
         */
        FramebufferSetDepthMboxMessage()
            : Mailbox::Message(0x48005, 4)
        {
            depth_bits = 0;
        }
    }; // class FramebufferSetDepthMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferSetDepthMboxMessage) == 16);

    class FramebufferSetPixelOrderMboxMessage : public Mailbox::Message 
    {
    public:
        enum PixelOrder : u32 
        {
            BGR = 0,
            RGB = 1
        }; // enum PixelOrder : u32

        PixelOrder pixel_order;

        /**
         * @brief Construct a new Framebuffer Set Pixel Order Mbox Message object
         * 
         */
        FramebufferSetPixelOrderMboxMessage()
            : Mailbox::Message(0x48006, 4)
        {
            pixel_order = PixelOrder::BGR;
        }
    }; // class FramebufferSetPixelOrderMboxMessage : public Mailbox::Message 
    static_assert(sizeof(FramebufferSetPixelOrderMboxMessage) == 16);

    class FramebufferAllocateBufferMboxMessage : public Mailbox::Message 
    {
    public:
        union 
        {
            u32 alignment;
            u32 address;
        };

        u32 size = 0;

        /**
         * @brief Construct a new Framebuffer Allocate Buffer Mbox Message object
         * 
         */
        FramebufferAllocateBufferMboxMessage()
            : Mailbox::Message(0x40001, 8)
        {
            alignment = 0;
            size = 0;
        }
    }; // class FramebufferAllocateBufferMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferAllocateBufferMboxMessage) == 20);

    class FramebufferGetPithMboxMessage : public Mailbox::Message 
    {
    public:
        u32 pitch;

        /**
         * @brief Construct a new Framebuffer Get Pith Mbox Message object
         * 
         */
        FramebufferGetPithMboxMessage()
            : Mailbox::Message(0x40008, 4)
        {
            pitch = 0;
        }
    }; // class FramebufferGetPithMboxMessage : public Mailbox::Message 

    static_assert(sizeof(FramebufferGetPithMboxMessage) == 16);

} // namespace Kernel