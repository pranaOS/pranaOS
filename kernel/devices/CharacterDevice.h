/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/Device.h>

namespace Kernel {

class CharacterDevice : public Device {
public:
    virtual ~CharacterDevice() override;

protected:
    CharacterDevice(unsigned major, unsigned minor)
        : Device(major, minor)
    {
    }

private:
    virtual bool is_character_device() const final { return true; }
};

}
