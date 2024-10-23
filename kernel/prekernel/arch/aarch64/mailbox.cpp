/**
 * @file mailbox.cpp
 * @author Krisna Pranav
 * @brief Mailbox
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/mmio.h>
#include <kernel/prekernel/arch/aarch64/mailbox.h>

namespace Prekernel 
{
    constexpr u32 MBOX_BASE_OFFSET = 0xB880;
    constexpr u32 MBOX_0 = MBOX_BASE_OFFSET;
    constexpr u32 MBOX_1 = MBOX_BASE_OFFSET + 0x20;

    constexpr u32 MBOX_READ_DATA = MBOX_0;
    constexpr u32 MBOX_READ_POLL = MBOX_0 + 0x10;
    constexpr u32 MBOX_READ_SENDER = MBOX_0 + 0x14;
    constexpr u32 MBOX_READ_STATUS = MBOX_0 + 0x18;
    constexpr u32 MBOX_READ_CONFIG = MBOX_0 + 0x1C;

    constexpr u32 MBOX_WRITE_DATA = MBOX_1;
    constexpr u32 MBOX_WRITE_STATUS = MBOX_1 + 0x18;

    constexpr u32 MBOX_RESPONSE_SUCCESS = 0x8000'0000;
    constexpr u32 MBOX_RESPONSE_PARTIAL = 0x8000'0001;
    constexpr u32 MBOX_REQUEST = 0;
    constexpr u32 MBOX_FULL = 0x8000'0000;
    constexpr u32 MBOX_EMPTY = 0x4000'0000;

    constexpr int ARM_TO_VIDEOCORE_CHANNEL = 8;

    /**
     * @brief Construct a new Mailbox::Message::Message object
     * 
     * @param tag 
     * @param arguments_size 
     */
    Mailbox::Message::Message(u32 tag, u32 arguments_size)
    {
        m_tag = tag;
        m_arguments_size = arguments_size;
        m_command_tag = MBOX_REQUEST;
    }

    /**
     * @brief Construct a new Mailbox::MessageHeader::MessageHeader object
     * 
     */
    Mailbox::MessageHeader::MessageHeader()
    {
        m_message_queue_size = 0;
        m_command_tag = MBOX_REQUEST;
    }

    /**
     * @return true 
     * @return false 
     */
    bool Mailbox::MessageHeader::success() const
    {
        return m_command_tag == MBOX_RESPONSE_SUCCESS;
    }

    /**
     * @return Mailbox& 
     */
    Mailbox& Mailbox::the()
    {
        static Mailbox instance;
        return instance;
    }

    /**
     * @param mmio 
     */
    static void wait_until_we_can_write(MMIO& mmio)
    {
        while (mmio.read(MBOX_WRITE_STATUS) & MBOX_FULL)
            ;
    }

    /**
     * @param mmio 
     */
    static void wait_for_reply(MMIO& mmio)
    {
        while (mmio.read(MBOX_READ_STATUS) & MBOX_EMPTY)
            ;
    }

    /**
     * @param queue 
     * @param queue_size 
     * @return true 
     * @return false 
     */
    bool Mailbox::send_queue(void* queue, u32 queue_size) const
    {
        const u32 channel = ARM_TO_VIDEOCORE_CHANNEL;

        auto message_header = reinterpret_cast<MessageHeader*>(queue);
        message_header->set_queue_size(queue_size);

        auto& mmio = MMIO::the();

        wait_until_we_can_write(mmio);

        u32 request = static_cast<u32>(reinterpret_cast<FlatPtr>(queue) & ~0xF) | (channel & 0xF);
        mmio.write(MBOX_WRITE_DATA, request);

        for (;;) {
            wait_for_reply(mmio);

            u32 response = mmio.read(MBOX_READ_DATA);
            
            if (response == request)
                return message_header->success();
        }

        return true;
    }
} // namespace Kernel