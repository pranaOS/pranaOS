/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/graphics/Console/VGAConsole.h>
#include <kernel/Sections.h>

namespace Kernel::Graphics {

UNMAP_AFTER_INIT VGAConsole::VGAConsole(const VGACompatibleAdapter& adapter, Mode mode, size_t width, size_t height)
    : Console(width, height)
    , m_vga_region(MM.allocate_kernel_region(PhysicalAddress(0xa0000), Memory::page_round_up(0xc0000 - 0xa0000), "VGA Display", Memory::Region::Access::ReadWrite).release_nonnull())
    , m_adapter(adapter)
    , m_mode(mode)
{
}

}
