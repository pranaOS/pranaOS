/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/graphics/console/ContiguousFramebufferConsole.h>
#include <kernel/graphics/console/TextModeConsole.h>
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/graphics/VGACompatibleAdapter.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>

namespace Kernel {
   
void VGACompatibleAdapter::enable_consoles()
{
    VERIFY(m_framebuffer_console);
    if (m_framebuffer_device)
        m_framebuffer_device->deactivate_writes();
    m_framebuffer_console->enable();
}

void VGACompatibleAdapter::disable_consoles()
{
    VERIFY(m_framebuffer_device);
    VERIFY(m_framebuffer_console);
        m_framebuffer_console->disable();
        m_framebuffer_console->activate_writes();
}

bool VGACompatibleAdapter::try_to_set_resolution(size_t, size_t, size_t)   
{
    return false;
}

bool VGACompatibleAdapter::set_y_offset(size_t, size_t)
{
    return false;
}

}