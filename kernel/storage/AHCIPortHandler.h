/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <kernel/devices/Device.h>
#include <kernel/IO.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/locking/Mutex.h>
#include <kernel/memory/PhysicalPage.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/Random.h>
#include <kernel/Sections.h>
#include <kernel/storage/AHCIController.h>
#include <kernel/storage/AHCIPort.h>
#include <kernel/storage/StorageDevice.h>
#include <kernel/WaitQueue.h>

namespace Kernel {

class AsyncBlockDeviceRequest;

class AHCIController;
class AHCIPort;
class AHCIPortHandler final : public RefCounted<AHCIPortHandler>
    , public IRQHandler {
    friend class AHCIController;
    friend class SATADiskDevice;

public:
    UNMAP_AFTER_INIT static NonnullRefPtr<AHCIPortHandler> create(AHCIController&, u8 irq, AHCI::MaskedBitField taken_ports);
    virtual ~AHCIPortHandler() override;

    RefPtr<StorageDevice> device_at_port(size_t port_index) const;
    virtual StringView purpose() const override { return "SATA Port Handler"; }

    AHCI::HBADefinedCapabilities hba_capabilities() const;
    NonnullRefPtr<AHCIController> hba_controller() const { return m_parent_controller; }
    PhysicalAddress get_identify_metadata_physical_region(u32 port_index) const;

    bool is_responsible_for_port_index(u32 port_index) const { return m_taken_ports.is_set_at(port_index); }

private:
    UNMAP_AFTER_INIT AHCIPortHandler(AHCIController&, u8 irq, AHCI::MaskedBitField taken_ports);

    virtual bool handle_irq(const RegisterState&) override;

    enum class Direction : u8 {
        Read,
        Write,
    };

    AHCI::MaskedBitField create_pending_ports_interrupts_bitfield() const;

    void start_request(AsyncBlockDeviceRequest&, bool, bool, u16);
    void complete_current_request(AsyncDeviceRequest::RequestResult);

    void enumerate_ports(Function<void(const AHCIPort&)> callback) const;
    RefPtr<AHCIPort> port_at_index(u32 port_index) const;

    HashMap<u32, NonnullRefPtr<AHCIPort>> m_handled_ports;
    NonnullRefPtr<AHCIController> m_parent_controller;
    NonnullRefPtrVector<Memory::PhysicalPage> m_identify_metadata_pages;
    AHCI::MaskedBitField m_taken_ports;
    AHCI::MaskedBitField m_pending_ports_interrupts;
};
}
