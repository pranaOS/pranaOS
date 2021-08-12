/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <base/RefPtr.h>
#include <kernel/bus/pci/Access.h>
#include <kernel/bus/pci/DeviceController.h>
#include <kernel/devices/Device.h>
#include <kernel/IO.h>
#include <kernel/locking/Mutex.h>
#include <kernel/memory/PhysicalPage.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/Random.h>
#include <kernel/WaitQueue.h>

namespace Kernel {

class AsyncBlockDeviceRequest;
class StorageDevice;
class StorageController : public RefCounted<StorageController> {
    BASE_MAKE_ETERNAL

public:
    virtual ~StorageController() = default;

    virtual RefPtr<StorageDevice> device(u32 index) const = 0;
    virtual size_t devices_count() const = 0;

protected:
    virtual void start_request(const StorageDevice&, AsyncBlockDeviceRequest&) = 0;

protected:
    virtual bool reset() = 0;
    virtual bool shutdown() = 0;

    virtual void complete_current_request(AsyncDeviceRequest::RequestResult) = 0;
};
}
