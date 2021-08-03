/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/Types.h>
#include <kernel/graphics/Console/Console.h>
#include <kernel/graphics/VGACompatibleAdapter.h>

namespace Kernel::Graphics {
class VGAConsole : public Console {
public:

    enum class Mode {
        TextMode = 1, 
        Colored256,   
        Colored16,    
    };

public:
    static NonnullRefPtr<VGAConsole> initialize(const VGACompatibleAdapter&, Mode, size_t width, size_t height);

    virtual bool is_hardware_paged_capable() const override { return false; }
    virtual bool has_hardware_cursor() const override { return false; }
    virtual void flush(size_t, size_t, size_t, size_t) override { }

    virtual ~VGAConsole() = default;

protected:
    VGAConsole(const VGACompatibleAdapter&, Mode, size_t width, size_t height);

    NonnullOwnPtr<Region> m_vga_region;
    NonnullRefPtr<VGACompatibleAdapter> m_adapter;
    const Mode m_mode;
};
}