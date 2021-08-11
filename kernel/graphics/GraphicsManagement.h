/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullOwnPtr.h>
#include <base/NonnullRefPtr.h>
#include <base/NonnullRefPtrVector.h>
#include <base/Types.h>
#include <kernel/bus/pci/Definitions.h>
#include <kernel/graphics/console/Console.h>
#include <kernel/graphics/GraphicsDevice.h>
#include <kernel/graphics/VGACompatibleAdapter.h>
#include <kernel/graphics/virtiogpu/GraphicsAdapter.h>
#include <kernel/Memory/Region.h>

namespace Kernel {

class BochsGraphicsAdapter;
class IntelNativeGraphicsAdapter;
class VGACompatibleAdapter;
class GraphicsManagement {
    friend class BochsGraphicsAdapter;
    friend class IntelNativeGraphicsAdapter;
    friend class VGACompatibleAdapter;
    friend class Graphics::VirtIOGPU::GraphicsAdapter;
    BASE_MAKE_ETERNAL

public:
    static GraphicsManagement& the();
    static bool is_initialized();
    bool initialize();

    unsigned allocate_minor_device_number() { return m_current_minor_number++; };
    GraphicsManagement();

    bool framebuffer_devices_allowed() const { return m_framebuffer_devices_allowed; }
    bool framebuffer_devices_exist() const;

    SpinLock<u8>& main_vga_lock() { return m_main_vga_lock; }
    RefPtr<Graphics::Console> console() const { return m_console; }

    void deactivate_graphical_mode();
    void activate_graphical_mode();

private:
    bool determine_and_initialize_graphics_device(const PCI::Address& address, PCI::ID id);
    NonnullRefPtrVector<GraphicsDevice> m_graphics_devices;
    RefPtr<Graphics::Console> m_console;


    RefPtr<VGACompatibleAdapter> m_vga_adapter;
    unsigned m_current_minor_number { 0 };
    const bool m_framebuffer_devices_allowed;

    SpinLock<u8> m_main_vga_lock;
};

}
