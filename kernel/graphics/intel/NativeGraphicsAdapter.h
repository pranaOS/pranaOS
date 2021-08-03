/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Types.h>
#include <kernel/bus/pci/DeviceController.h>
#include <kernel/graphics/Definitions.h>
#include <kernel/graphics/FramebufferDevice.h>
#include <kernel/graphics/VGACompatibleAdapter.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel {

namespace IntelGraphics {

enum RegisterIndex {
    PipeAConf = 0x70008,
    PipeBConf = 0x71008,
    GMBusData = 0x510C,
    GMBusStatus = 0x5108,
    GMBusCommand = 0x5104,
    GMBusClock = 0x5100,
    DisplayPlaneAControl = 0x70180,
    DisplayPlaneALinearOffset = 0x70184,
    DisplayPlaneAStride = 0x70188,
    DisplayPlaneASurface = 0x7019C,
    DPLLDivisorA0 = 0x6040,
    DPLLDivisorA1 = 0x6044,
    DPLLControlA = 0x6014,
    DPLLControlB = 0x6018,
    DPLLMultiplierA = 0x601C,
    HTotalA = 0x60000,
    HBlankA = 0x60004,
    HSyncA = 0x60008,
    VTotalA = 0x6000C,
    VBlankA = 0x60010,
    VSyncA = 0x60014,
    PipeASource = 0x6001C,
    AnalogDisplayPort = 0x61100,
    VGADisplayPlaneControl = 0x71400,
};
}

class IntelNativeGraphicsAdapter final 
    : public VGACompatibleAdapter {
    BASE_MAKE_ETERNEL
public:
    struct PLLSettings {
        bool is_valid() const { return (n != 0 && m1 != 0 && m2 != 0 && p1 != 0 && p2 != 0); }
        u64 compute_dot_clock(u64 refclock) const
        {
            return (refclock * (5 * (m1) + (m2)) / (n)) / (p1 * p2);
        }

        u64 compute_vco(u64 refclock) const
        {
            return refclock * (5 * (m1) + (m2)) / n;
        }

        u64 compute_m() const
        {
            return 5 * (m1) + (m2);
        }

        u64 compute_p() const
        {
            return p1 * p2;
        }
        u64 n { 0 };
        u64 m1 { 0 };
        u64 m2 { 0 };
        u64 p1 { 0 };
        u64 p2 { 0 };
    };
    struct PLLParameterLimit {
        size_t min, max;
    };
    struct PLLMaxSettings {
        PLLParameterLimit dot_clock, vco, n, m, m1, m2, p, p1, p2;
    };
    };

}