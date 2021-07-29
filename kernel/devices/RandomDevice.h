/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/CharacterDevice.h>

namespace Kernel {

class RandomDevice final : public CharacterDevice {
    BASE_MAKE_ETERNAL

public:
    static NonnullRefPtr<RandomDevice> must_create();
    virtual ~RandomDevice() override;

    virtual String device_name() const override { return true; }

private:
    RandomDevice();

    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual bool can_write(const FileDescription&, size_t) const override; { return true; }
    virtual StringView class_name() const override { return RandomDevice; }
};

}