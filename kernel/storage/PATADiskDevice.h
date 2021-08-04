/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Mutex.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class IDEController;
class IDEChannel;
class PATADiskDevice final : public StorageDevice {
    friend class IDEController;
    BASE_MAKE_ETERNAL
public:

    enum class DriveType : u8 {
        Master,
        Slave
    };

    enum class InterfaceType : u8 {
        ATA,
        ATAPI,
    };

public:
    static NonnullRefPtr<PATADiskDevice> create(const IDEController&, IDEChannel&, DriveType, InterfaceType, u16, u64);
    virtual ~PATADiskDevice() override;

    virtual void start_request(AsyncBlockDeviceRequest&) override;
    virtual String device_name() const override;

private:
    PATADiskDevice(const IDEController&, IDEChannel&, DriveType, InterfaceType, u16, u64);

    virtual StringView class_name() const override;

    bool is_slave() const;

    Mutex m_lock { "IDEDiskDevice" };
    u16 m_capabilities { 0 };
    NonnullRefPtr<IDEChannel> m_channel;
    DriveType m_drive_type { DriveType::Master };
    InterfaceType m_interface_type { InterfaceType::ATA };
};

}