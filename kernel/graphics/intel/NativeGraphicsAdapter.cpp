/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// incldues
#include <kernel/graphics/console/ContiguousFramebufferConsole.h>
#include <kernel/graphics/Definitions.h>
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/graphics/intel/NativeGraphicsAdapter.h>
#include <kernel/IO.h>
#include <kernel/PhysicalAddress.h>

namespace Kerenl {

static constexpr IntelNativeGraphicsAdapter::PLLMaxSettings G35Limits {
    { 20'000'000, 400'000'000 },      
    { 1'400'000'000, 2'800'000'000 }, 
    { 3, 8 },                         
    { 70, 120 },                     
    { 10, 20 },                       
    { 5, 9 },                         
    { 5, 80 },                        
    { 1, 8 },                         
    { 5, 10 }                         
};

static constexpr u16 supported_models[] {
    0x29c2, 
};

static bool is_supported_model(u16 device_id)
{
    for (auto& id : supported_models) {
        if (id == device_id)
            return true;
    }
    return false;
}

#define DDC2_I2C_ADDRESS 0x50

RefPtr<IntelNativeGraphicsAdapter> IntelNativeGraphicsAdapter::initialize(PCI::Address address)
{
    auto id = PCI::get_id(address);
    VERIFY(id.vendor_id == 0x8086);
    if (!is_supported_model(id.device_id))
        return {};
    return adopt_ref(*new IntelNativeGraphicsAdapter(address));
}

static size_t compute_dac_multiplier(size_t pixel_clock_in_khz)
{
    dbgln_if(INTEL_GRAPHICS_DEBUG, "Intel native graphics: Pixel clock is {} KHz", pixel_clock_in_khz);
    VERIFY(pixel_clock_in_khz >= 25000);
    if (pixel_clock_in_khz >= 100000) {
        return 1;
    } else if (pixel_clock_in_khz >= 50000) {
        return 2;
    } else {
        return 4;
    }
}

static Graphics::Modesetting calculate_modesetting_from_edid(const Graphics::VideoInfoBlock& edid, size_t index)
{
    Graphics::Modesetting mode;
    VERIFY(edid.details[0].pixel_clock);
    mode.pixel_clock_in_khz = edid.details[0].pixel_clock * 10;

    size_t horizontal_active = edid.details[index].horizontal_active | ((edid.details[index].horizontal_active_blank_msb >> 4) << 8);
    size_t horizontal_blank = edid.details[index].horizontal_blank | ((edid.details[index].horizontal_active_blank_msb & 0xF) << 8);
    size_t horizontal_sync_offset = edid.details[index].horizontal_sync_offset | ((edid.details[index].sync_msb >> 6) << 8);
    size_t horizontal_sync_pulse = edid.details[index].horizontal_sync_pulse | (((edid.details[index].sync_msb >> 4) & 0x3) << 8);

    mode.horizontal.active = horizontal_active;
    mode.horizontal.sync_start = horizontal_active + horizontal_sync_offset;
    mode.horizontal.sync_end = horizontal_active + horizontal_sync_offset + horizontal_sync_pulse;
    mode.horizontal.total = horizontal_active + horizontal_blank;

    size_t vertical_active = edid.details[index].vertical_active | ((edid.details[index].vertical_active_blank_msb >> 4) << 8);
    size_t vertical_blank = edid.details[index].vertical_blank | ((edid.details[index].vertical_active_blank_msb & 0xF) << 8);
    size_t vertical_sync_offset = (edid.details[index].vertical_sync >> 4) | (((edid.details[index].sync_msb >> 2) & 0x3) << 4);
    size_t vertical_sync_pulse = (edid.details[index].vertical_sync & 0xF) | ((edid.details[index].sync_msb & 0x3) << 4);

    mode.vertical.active = vertical_active;
    mode.vertical.sync_start = vertical_active + vertical_sync_offset;
    mode.vertical.sync_end = vertical_active + vertical_sync_offset + vertical_sync_pulse;
    mode.vertical.total = vertical_active + vertical_blank;
    return mode;
}

static bool check_pll_settings(const IntelNativeGraphicsAdapter::PLLSettings& settings, size_t reference_clock, const IntelNativeGraphicsAdapter::PLLMaxSettings& limits)
{
    if (settings.n < limits.n.min || settings.n > limits.n.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "N is invalid {}", settings.n);
        return false;
    }
    if (settings.m1 < limits.m1.min || settings.m1 > limits.m1.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "m1 is invalid {}", settings.m1);
        return false;
    }
    if (settings.m2 < limits.m2.min || settings.m2 > limits.m2.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "m2 is invalid {}", settings.m2);
        return false;
    }
    if (settings.p1 < limits.p1.min || settings.p1 > limits.p1.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "p1 is invalid {}", settings.p1);
        return false;
    }

    if (settings.m1 <= settings.m2) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "m2 is invalid {} as it is bigger than m1 {}", settings.m2, settings.m1);
        return false;
    }

    auto m = settings.compute_m();
    auto p = settings.compute_p();

    if (m < limits.m.min || m > limits.m.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "m invalid {}", m);
        return false;
    }
    if (p < limits.p.min || p > limits.p.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "p invalid {}", p);
        return false;
    }

    auto dot = settings.compute_dot_clock(reference_clock);
    auto vco = settings.compute_vco(reference_clock);

    if (dot < limits.dot_clock.min || dot > limits.dot_clock.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "Dot clock invalid {}", dot);
        return false;
    }
    if (vco < limits.vco.min || vco > limits.vco.max) {
        dbgln_if(INTEL_GRAPHICS_DEBUG, "VCO clock invalid {}", vco);
        return false;
    }
    return true;
}

static size_t find_absolute_difference(u64 target_frequency, u64 checked_frequency)
{
    if (target_frequency >= checked_frequency)
        return target_frequency - checked_frequency;
    return checked_frequency - target_frequency;
}

}