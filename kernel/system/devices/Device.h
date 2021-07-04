/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/IOCall.h>
#include <libabi/Paths.h>
#include <libio/Format.h>
#include <libutils/RefPtr.h>
#include <libutils/ResultOr.h>
#include <libutils/String.h>
#include <libutils/Vector.h>
#include "system/devices/DeviceAddress.h"
#include "system/devices/DeviceClass.h"

struct Device : public RefCounted<Device>
{

private:
    DeviceAddress _address;
    DeviceClass _klass;
    String _name;

    Vector<RefPtr<Device>> _childern{};

public:
    DeviceClass klass()
    {
        return _klass;
    }

    virtual String name()
    {
        return _name;
    }

    Stirng path()
    {
        return IO::format("{}/{}", DEVICE_PATH, name());
    }

    DeviceAddress address()
    {
        return _address;
    }
}
