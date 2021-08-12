/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/CharacterDevice.h>

namespace Kernel {

class FullDevice final : public CharacterDevice {
    BASE_MAKE_ETERNAL
public:
    static NonnullRefPtr<FullDevice> must_create();
    virtual ~FullDevice() override;

    virtual mode_t required_mode() const override { return 0666; }
    virtual String device_name() const override { return "full"; }

private:
    FullDevice();

    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }
    virtual StringView class_name() const override { return "FullDevice"; }
};

}
