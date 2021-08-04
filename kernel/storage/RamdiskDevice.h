/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/Mutex.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class RamdiskController;

class RamdiskDevice final : public StorageDevice {
    friend class RamdiskController;
    BASE_MAKE_ETERNAL
public:
    static NonnullRefPtr<RamdiskDevice> create(const RamdiskController&, NonnullOwnPtr<Region>&& region, int major, int minor);
    RamdiskDevice(const RamdiskController&, NonnullOwnPtr<Region>&&, int major, int minor);
    virtual ~RamdiskDevice() override;

    virtual void start_request(AsyncBlockDeviceRequest&) override;

    virtual StringView class_name() const override;
    virtual String device_name() const override;

    bool is_slave() const;

    Mutex m_lock { "RamdiskDevice" };

    NonnullOwnPtr<Region> m_region;
};

}