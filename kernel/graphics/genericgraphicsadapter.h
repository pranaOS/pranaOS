/**
 * @file genericgraphicsadapter.h
 * @author Krisna Pranav
 * @brief Generic Graphics Adapter
 * @version 6.0
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <mods/weakable.h>
#include <kernel/bus/pci/definitions.h>
#include <kernel/device/blockdevice.h>
#include <kernel/physicaladdress.h>

namespace Kernel 
{
    class GenericGraphicsAdapter
        : public RefCounted<GenericGraphicsAdapter>
        , public Weakable<GenericGraphicsAdapter> {
    public:
        virtual ~GenericGraphicsAdapter() = default;
        virtual void initialize_framebuffer_devices() = 0;
        virtual void enable_consoles() = 0;
        virtual void disable_consoles() = 0;
        bool consoles_enabled() const { return m_consoles_enabled; }
        virtual bool framebuffer_devices_initialized() const = 0;

        virtual bool modesetting_capable() const = 0;
        virtual bool double_framebuffering_capable() const = 0;

        virtual bool vga_compatible() const = 0;

        virtual ErrorOr<ByteBuffer> get_edid(size_t output_port_index) const = 0;

        virtual bool try_to_set_resolution(size_t output_port_index, size_t width, size_t height) = 0;
        virtual bool set_y_offset(size_t output_port_index, size_t y) = 0;

    protected:
        GenericGraphicsAdapter() = default;

        bool m_consoles_enabled { false };
    };

} // namespace Kernel