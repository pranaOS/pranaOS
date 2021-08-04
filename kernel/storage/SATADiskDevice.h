/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/Mutex.h>
#include <kernel/storage/AHCIPort.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class AHCIController;
class SATADiskDevice final : public StorageDevice {
    friend class AHCIController;

public:
    enum class InterfaceType : u8 {
        SATA,
        SATAPI,
    };

public:
    static NonnullRefPtr<SATADiskDevice> create(const AHCIController&, const AHCIPort&, size_t sector_size, u64 max_addressable_block);
    virtual ~SATADiskDevice() override;

    virtual void start_request(AsyncBlockDeviceRequest&) override;
    virtual String device_name() const override;

private:
    SATADiskDevice(const AHCIController&, const AHCIPort&, size_t sector_size, u64 max_addressable_block);

    virtual StringView class_name() const override;
    NonnullRefPtr<AHCIPort> m_port;
};

}