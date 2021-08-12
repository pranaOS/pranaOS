/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/RefPtr.h>
#include <base/Types.h>
#include <kernel/storage/RamdiskDevice.h>
#include <kernel/storage/StorageController.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class AsyncBlockDeviceRequest;

class RamdiskController final : public StorageController {
    BASE_MAKE_ETERNAL
public:
public:
    static NonnullRefPtr<RamdiskController> initialize();
    virtual ~RamdiskController() override;

    virtual RefPtr<StorageDevice> device(u32 index) const override;
    virtual bool reset() override;
    virtual bool shutdown() override;
    virtual size_t devices_count() const override;
    virtual void start_request(const StorageDevice&, AsyncBlockDeviceRequest&) override;
    virtual void complete_current_request(AsyncDeviceRequest::RequestResult) override;

private:
    RamdiskController();

    NonnullRefPtrVector<RamdiskDevice> m_devices;
};
}
