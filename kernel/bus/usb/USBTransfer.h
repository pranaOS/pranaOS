/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/RefPtr.h>
#include <kernel/bus/usb/PacketTypes.h>
#include <kernel/bus/usb/USBPipe.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/PhysicalPage.h>
#include <kernel/vm/Region.h>

namespace Kernel::USB {

class Transfer : public RefCounted<Transfer> {
public:
    static RefPtr<Transfer> try_create(Pipe& pipe, u16 len);

public:
    Transfer() = delete;
    Transfer(Pipe& pipe, u16 len, AnonymousVMObject&);
    ~Transfer();

    void set_setup_packet(const USBRequestData& request);
    void set_complete() { m_complete = true; }
    void set_error_occurred() { m_error_occurred = true; }

    const USBRequestData& request() const { return m_request; }
    const Pipe& pipe() const { return m_pipe; }
    Pipe& pipe() { return m_pipe; }
    VirtualAddress buffer() const { return m_data_buffer->vaddr(); }
    PhysicalAddress buffer_physical() const { return m_data_buffer->physical_page(0)->paddr(); }
    u16 transfer_data_size() const { return m_transfer_data_size; }
    bool complete() const { return m_complete; }
    bool error_occurred() const { return m_error_occurred; }

private:
    Pipe& m_pipe;                    
    USBRequestData m_request;        
    OwnPtr<Region> m_data_buffer;    
    u16 m_transfer_data_size { 0 };  
    bool m_complete { false };       
    bool m_error_occurred { false }; 
};
}