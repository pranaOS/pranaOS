/*
 * Copyright (c) 2020, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/Ptr32.h>
#include <base/Types.h>
#include <kernel/bus/usb/USBTransfer.h>

namespace Kernel::USB {

enum class PacketID : u8 {
    IN = 0x69,
    OUT = 0xe1,
    SETUP = 0x2d
};

constexpr u16 TD_CONTROL_STATUS_ACTLEN = 0x7ff;
constexpr u8 TD_CONTROL_STATUS_ACTIVE_SHIFT = 23;
constexpr u8 TD_CONTROL_STATUS_INT_ON_COMPLETE_SHIFT = 24;
constexpr u8 TD_CONTROL_STATUS_ISOCHRONOUS_SHIFT = 25;
constexpr u8 TD_CONTROL_STATUS_LS_DEVICE_SHIFT = 26;
constexpr u8 TD_CONTROL_STATUS_ERR_CTR_SHIFT_SHIFT = 27;
constexpr u8 TD_CONTROL_STATUS_SPD_SHIFT = 29;

constexpr u8 TD_TOKEN_PACKET_ID_SHIFT = 0;
constexpr u8 TD_TOKEN_DEVICE_ADDR_SHIFT = 8;
constexpr u8 TD_TOKEN_ENDPOINT_SHIFT = 15;
constexpr u8 TD_TOKEN_DATA_TOGGLE_SHIFT = 19;
constexpr u8 TD_TOKEN_MAXLEN_SHIFT = 21;

struct QueueHead;
struct alignas(16) TransferDescriptor final {
    enum LinkPointerBits {
        Terminate = 1,
        QHSelect = 2,
        DepthFlag = 4,
    };

    enum StatusBits {
        Reserved = (1 << 16),
        BitStuffError = (1 << 17),
        CRCTimeoutError = (1 << 18),
        NAKReceived = (1 << 19),
        BabbleDetected = (1 << 20),
        DataBufferError = (1 << 21),
        Stalled = (1 << 22),
        Active = (1 << 23),
        ErrorMask = BitStuffError | CRCTimeoutError | NAKReceived | BabbleDetected | DataBufferError | Stalled
    };

    enum ControlBits {
        InterruptOnComplete = (1 << 24),
        IsochronousSelect = (1 << 25),
        LowSpeedDevice = (1 << 26),
        ShortPacketDetect = (1 << 29),
    };

    TransferDescriptor() = delete;
    TransferDescriptor(u32 paddr)
        : m_paddr(paddr)
    {
    }
    ~TransferDescriptor() = delete; 

    u32 link_ptr() const { return m_link_ptr; }
    u32 paddr() const { return m_paddr; }
    u32 status() const { return m_control_status; }
    u32 token() const { return m_token; }
    u32 buffer_ptr() const { return m_buffer_ptr; }
    u16 actual_packet_length() const { return (m_control_status + 1) & 0x7ff; }

    bool in_use() const { return m_in_use; }
    bool stalled() const { return m_control_status & StatusBits::Stalled; }
    bool last_in_chain() const { return m_link_ptr & LinkPointerBits::Terminate; }
    bool active() const { return m_control_status & StatusBits::Active; }

    void set_active()
    {
        u32 ctrl = m_control_status;
        ctrl |= StatusBits::Active;
        m_control_status = ctrl;
    }

    void set_isochronous()
    {
        u32 ctrl = m_control_status;
        ctrl |= ControlBits::IsochronousSelect;
        m_control_status = ctrl;
    }

    void set_interrupt_on_complete()
    {
        u32 ctrl = m_control_status;
        ctrl |= ControlBits::InterruptOnComplete;
        m_control_status = ctrl;
    }

    void set_lowspeed()
    {
        u32 ctrl = m_control_status;
        ctrl |= ControlBits::LowSpeedDevice;
        m_control_status = ctrl;
    }

    void set_error_retry_counter(u8 num_retries)
    {
        VERIFY(num_retries <= 3);
        u32 ctrl = m_control_status;
        ctrl |= (num_retries << 27);
        m_control_status = ctrl;
    }

    void set_short_packet_detect()
    {
        u32 ctrl = m_control_status;
        ctrl |= ControlBits::ShortPacketDetect;
        m_control_status = ctrl;
    }

    void set_control_status(u32 control_status) { m_control_status = control_status; }
    void set_in_use(bool in_use) { m_in_use = in_use; }
    void set_max_len(u16 max_len)
    {
        VERIFY(max_len < 0x500 || max_len == 0x7ff);
        m_token |= (max_len << 21);
    }

    void set_device_endpoint(u8 endpoint)
    {
        VERIFY(endpoint <= 0xf);
        m_token |= (endpoint << 18);
    }

    void set_device_address(u8 address)
    {
        VERIFY(address <= 0x7f);
        m_token |= (address << 8);
    }

    void set_data_toggle(bool toggle)
    {
        m_token |= ((toggle ? (1 << 19) : 0));
    }

    void set_packet_id(PacketID pid) { m_token |= static_cast<u32>(pid); }
    void link_queue_head(u32 qh_paddr)
    {
        m_link_ptr = qh_paddr;
        m_link_ptr |= LinkPointerBits::QHSelect;
    }

    void print()
    {
        dbgln("UHCI: TD({:#04x}) @ {:#04x}: link_ptr={:#04x}, status={:#04x}, token={:#04x}, buffer_ptr={:#04x}", this, m_paddr, m_link_ptr, (u32)m_control_status, m_token, m_buffer_ptr);

        dbgln("UHCI: TD({:#04x}) @ {:#04x}: link_ptr={}{}{}, status={}{}{}{}{}{}{}",
            this,
            m_paddr,
            (last_in_chain()) ? "T " : "",
            (m_link_ptr & static_cast<u32>(LinkPointerBits::QHSelect)) ? "QH " : "",
            (m_link_ptr & static_cast<u32>(LinkPointerBits::DepthFlag)) ? "Vf " : "",
            (m_control_status & static_cast<u32>(StatusBits::BitStuffError)) ? "BITSTUFF " : "",
            (m_control_status & static_cast<u32>(StatusBits::CRCTimeoutError)) ? "CRCTIMEOUT " : "",
            (m_control_status & static_cast<u32>(StatusBits::NAKReceived)) ? "NAK " : "",
            (m_control_status & static_cast<u32>(StatusBits::BabbleDetected)) ? "BABBLE " : "",
            (m_control_status & static_cast<u32>(StatusBits::DataBufferError)) ? "DATAERR " : "",
            (stalled()) ? "STALL " : "",
            (active()) ? "ACTIVE " : "");
    }

    TransferDescriptor* next_td() { return m_next_td; }
    const TransferDescriptor* next_td() const { return m_next_td; }
    void set_next_td(TransferDescriptor* td) { m_next_td = td; }

    TransferDescriptor* prev_td() { return m_prev_td; }
    const TransferDescriptor* prev_td() const { return m_prev_td; }
    void set_previous_td(TransferDescriptor* td) { m_prev_td = td; }

    void insert_next_transfer_descriptor(TransferDescriptor* td)
    {
        m_link_ptr = td->paddr();
        td->set_previous_td(this);
        set_next_td(td);

        m_link_ptr |= static_cast<u32>(LinkPointerBits::DepthFlag);
    }

        void terminate() { m_link_ptr |= static_cast<u32>(LinkPointerBits::Terminate); }

    void set_buffer_address(Ptr32<u8> buffer)
    {
        u8* buffer_address = &*buffer;
        m_buffer_ptr = reinterpret_cast<uintptr_t>(buffer_address);
    }

    void set_token(u32 token)
    {
        m_token = token;
    }

    void set_status(u32 status)
    {
        m_control_status = status;
    }

    void free()
    {
        m_link_ptr = 0;
        m_control_status = 0;
        m_token = 0;
        m_in_use = false;
    }

private:
    u32 m_link_ptr;                
    volatile u32 m_control_status; 
    u32 m_token;                   
    u32 m_buffer_ptr;              

    u32 m_paddr;                                     
    Ptr32<TransferDescriptor> m_next_td { nullptr }; 
    Ptr32<TransferDescriptor> m_prev_td { nullptr }; 
    bool m_in_use;                                   
};

static_assert(sizeof(TransferDescriptor) == 32); 

struct alignas(16) QueueHead {
    enum class LinkPointerBits : u32 {
        Terminate = 1,
        QHSelect = 2,
    };

    QueueHead() = delete;
    QueueHead(u32 paddr)
        : m_paddr(paddr)
    {
    }
    ~QueueHead() = delete; 

    u32 link_ptr() const { return m_link_ptr; }
    u32 element_link_ptr() const { return m_element_link_ptr; }
    u32 paddr() const { return m_paddr; }
    bool in_use() const { return m_in_use; }

    void set_in_use(bool in_use) { m_in_use = in_use; }
    void set_link_ptr(u32 val) { m_link_ptr = val; }

    QueueHead* next_qh() { return m_next_qh; }
    const QueueHead* next_qh() const { return m_next_qh; }
    void set_next_qh(QueueHead* qh) { m_next_qh = qh; }

    QueueHead* prev_qh() { return m_prev_qh; }
    const QueueHead* prev_qh() const { return m_prev_qh; }
    void set_previous_qh(QueueHead* qh)
    {
        m_prev_qh = qh;
    }

    void link_next_queue_head(QueueHead* qh)
    {
        m_link_ptr = qh->paddr();
        m_link_ptr |= static_cast<u32>(LinkPointerBits::QHSelect);
    }

    void attach_transfer_queue(QueueHead& qh)
    {
        m_element_link_ptr = qh.paddr();
        m_element_link_ptr = m_element_link_ptr | static_cast<u32>(LinkPointerBits::QHSelect);
    }

    void free_transfer_queue([[maybe_unused]] QueueHead* qh)
    {
        TODO();
    }

    void terminate_with_stray_descriptor(TransferDescriptor* td)
    {
        m_link_ptr = td->paddr();
        m_link_ptr |= static_cast<u32>(LinkPointerBits::Terminate);
    }

    void attach_transfer_descriptor_chain(TransferDescriptor* td)
    {
        m_first_td = td;
        m_element_link_ptr = td->paddr();
    }

    TransferDescriptor* get_first_td()
    {
        return m_first_td;
    }

    void terminate() { m_link_ptr |= static_cast<u32>(LinkPointerBits::Terminate); }

    void terminate_element_link_ptr()
    {
        m_element_link_ptr = static_cast<u32>(LinkPointerBits::Terminate);
    }

    void set_transfer(Transfer* transfer)
    {
        m_transfer = transfer;
    }

    Transfer* transfer()
    {
        return m_transfer;
    }

    void print()
    {
        dbgln("UHCI: QH({:#04x}) @ {:#04x}: link_ptr={:#04x}, element_link_ptr={:#04x}", this, m_paddr, m_link_ptr, (FlatPtr)m_element_link_ptr);
    }

    void free()
    {
        m_link_ptr = 0;
        m_element_link_ptr = 0;
        m_first_td = nullptr;
        m_transfer = nullptr;
        m_in_use = false;
    }

}