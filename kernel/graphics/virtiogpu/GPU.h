/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/BinaryBufferWriter.h>
#include <base/DistinctNumeric.h>
#include <kernel/devices/BlockDevice.h>
#include <kernel/graphics/virtiogpu/Protocol.h>
#include <kernel/virtio/VirtIO.h>
#include <kernel/virtio/VirtIOQueue.h>

#define VIRTIO_GPU_F_VIRGL (1 << 0)
#define VIRTIO_GPU_F_EDID (1 << 1)

#define VIRTIO_GPU_FLAG_FENCE (1 << 0)

#define CONTROLQ 0
#define CURSORQ 1

#define MAX_VIRTIOGPU_RESOLUTION_WIDTH 3840
#define MAX_VIRTIOGPU_RESOLUTION_HEIGHT 2160

#define VIRTIO_GPU_EVENT_DISPLAY (1 << 0)

namespace Kernel::Graphics::VirtIOGPU {

class Console;
class FrameBufferDevice;

TYPEDEF_DISTINCT_ORDERED_ID(u32, ResourceID);
TYPEDEF_DISTINCT_ORDERED_ID(u32, ScanoutID);

class GPU final
    : public VirtIODevice
    , public RefCounted<GPU> {
    friend class FrameBufferDevice;

public:
    GPU(PCI::Address);
    virtual ~GPU() override;

    void create_framebuffer_devices();
    template<typename F>
    IterationDecision for_each_framebuffer(F f)
    {
        for (auto& scanout : m_scanouts) {
            if (!scanout.framebuffer)
                continue;
            IterationDecision decision = f(*scanout.framebuffer, *scanout.console);
            if (decision != IterationDecision::Continue)
                return decision;
        }
        return IterationDecision::Continue;
    }

    RefPtr<Console> default_console()
    {
        if (m_default_scanout.has_value())
            return m_scanouts[m_default_scanout.value().value()].console;
        return {};
    }

    };
}