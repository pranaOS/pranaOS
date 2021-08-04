/*
 * Copyright (c) 2021, NukeWilliams
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/OwnPtr.h>
#include <base/RefPtr.h>
#include <base/Types.h>
#include <kernel/storage/IDEChannel.h>
#include <kernel/storage/StorageController.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class AsyncBlockDeviceRequest;

class IDEController final : public StorageController
    , public PCI::DeviceController {
    BASe_MAKE_ETERNAL
public:
    static NonnullRefPtr<IDEController> initialize(PCI::Address address, bool force_pio);
    virtual ~IDEController() override;

    virtual RefPtr<StorageDevice> device(u32 index) const override;
    virtual bool reset() override;
    virtual bool shutdown() override;
    virtual size_t devices_count() const override;
    virtual void start_request(const StorageDevice&, AsyncBlockDeviceRequest&) override;
    virtual void complete_current_request(AsyncDeviceRequest::RequestResult) override;

    bool is_bus_master_capable() const;
    bool is_pci_native_mode_enabled() const;

private:
    bool is_pci_native_mode_enabled_on_primary_channel() const;
    bool is_pci_native_mode_enabled_on_secondary_channel() const;
    IDEController(PCI::Address address, bool force_pio);

    RefPtr<StorageDevice> device_by_channel_and_position(u32 index) const;
    void initialize(bool force_pio);
    void detect_disks();

    NonnullRefPtrVector<IDEChannel> m_channels;
};
}
