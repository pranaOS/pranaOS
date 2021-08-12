/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullRefPtr.h>
#include <base/NonnullRefPtrVector.h>
#include <kernel/bus/usb/USBController.h>

namespace Kernel::USB {

class USBManagement {
    BASE_MAKE_ETERNAL;

public:
    USBManagement();
    static bool initialized();
    static void initialize();
    static USBManagement& the();

private:
    void enumerate_controllers();

    USBController::List m_controllers;
};

}
