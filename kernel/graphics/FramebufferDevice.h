/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullOwnPtr.h>
#include <base/String.h>
#include <base/Types.h>
#include <kernel/devices/BlockDevice.h>
#include <kernel/graphics/GraphicsDevice.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/SpinLock.h>
#include <kernel/vm/AnonymousVMObject.h>


namespace Kernel {

class FramebufferDevice : public BlockDevice {
    BASE_MAKE_ETERNAL
public:
    static NonnullRefPtr<FramebufferDevice> create(const GraphicsDevice&, size_t, PhysicalAddress, size_t, size_t, size_t);

    virtual KResult ioctl(FileDescription&, unsigned request, Userspace<void*> arg) override;
    virtual KResultOr<Region*> mmap(Process&, FileDescription&, const Range&, u64 offset, int prot, bool shared) override;

    virtual mode_t required_mode() const override { return 0660; }
    virtual String device_name() const override;

    virtual void deactivate_writes();
    virtual void activate_writes();
    size_t framebuffer_size_in_bytes() const;

    virtual ~FramebufferDevice() {};
    void initialize();


}