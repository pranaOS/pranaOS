/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/BlockDevice.h>
#include <kernel/devices/KCOVInstance.h>

namespace Kernel {
class KCOVDevice final : public BlockDevice {
    BASE_MAKE_ETERNAL

public:
    static HashMap<ProcessID, KCOVInstance*>* proc_instance;
    static HashMap<ThreadID, KCOVInstance*>* thread_instance;

    static NonnullRefPtr<KCOVDevice> must_create();
    static void free_thread();
    static void free_process();

    KResultOr<Memory::Region*> mmap(Process&, FileDescription&, Memory::VirtualRange const&, u64 offset, int prot, bool shared) override;
    KResultOr<NonnullRefPtr<FileDescription>> open(int options) override;

    // ^Device
    virtual mode_t required_mode() const override { return 0660; }
    virtual String device_name() const override;

protected:
    virtual StringView class_name() const override { return "KCOVDevice"; }

    virtual bool can_read(const FileDescription&, size_t) const override final { return true; }
    virtual bool can_write(const FileDescription&, size_t) const override final { return true; }
    virtual void start_request(AsyncBlockDeviceRequest& request) override final { request.complete(AsyncDeviceRequest::Failure); }
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override { return EINVAL; }
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override { return EINVAL; }
    virtual KResult ioctl(FileDescription&, unsigned request, Userspace<void*> arg) override;

private:
    KCOVDevice();
};

}
