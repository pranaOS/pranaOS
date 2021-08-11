/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>
#include <kernel/devices/CharacterDevice.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel {

class MemoryDevice final : public CharacterDevice {
    BASE_MAKE_ETERNAL
public:
    static NonnullRefPtr<MemoryDevice> must_create();
    ~MemoryDevice();

    virtual KResultOr<Memory::Region*> mmap(Process&, FileDescription&, Memory::VirtualRange const&, u64 offset, int prot, bool shared) override;

    virtual mode_t required_mode() const override { return 0660; }
    virtual String device_name() const override { return "mem"; };

private:
    MemoryDevice();
    virtual StringView class_name() const override { return "MemoryDevice"; }
    virtual bool can_read(const FileDescription&, size_t) const override { return true; }
    virtual bool can_write(const FileDescription&, size_t) const override { return false; }
    virtual bool is_seekable() const override { return true; }
    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override { return EINVAL; }

    virtual void did_seek(FileDescription&, off_t) override;

    bool is_allowed_range(PhysicalAddress, Memory::VirtualRange const&) const;
};

}
