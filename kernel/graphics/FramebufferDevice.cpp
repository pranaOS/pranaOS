/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Checked.h>
#include <kernel/Debug.h>
#include <kernel/graphics/FramebufferDevice.h>
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/Process.h>
#include <kernel/Sections.h>
#include <kernel/VM/AnonymousVMObject.h>
#include <kernel/VM/MemoryManager.h>
#include <libc/errno_numbers.h>
#include <libc/sys/ioctl_numbers.h>

#define MAX_RESOLUTION_WITH 4096
#define MAX_RESOLUTION_HEIGHT 2160

namespace Kernel {

NonnullRefPtr<FramebufferDevice> FramebufferDevice::create(const GraphicsDevice& adapter, size_t output_port_index, PhysicalAddress paddr, size_t width, size_t height, size_t pitch)
{
    return adopt_ref(*new FramebufferDevice(adapter, output_port_index, paddr, width, height, pitch));
}

KResultOr<Region*> FramebufferDevice::mmap(Process& process, FileDescription&, const Range& range, u64 offset, int prot, bool shared)
{
    ScopedSpinLock lock(m_activation_lock);
    REQUIRE_PROMISE(video);
    if (!shared)
        return ENODEV;
    if (offset != 0)
        return ENXIO;
    if (range.size() != page_round_up(framebuffer_size_in_bytes()))
        return EOVERFLOW;

    auto vmobject = AnonymousVMObject::try_create_for_physical_range(m_framebuffer_address, page_round_up(framebuffer_size_in_bytes()));
    if (!vmobject)
        return ENOMEM;
    m_userspace_real_framebuffer_vmobject = vmobject;

    m_real_framebuffer_vmobject = AnonymousVMObject::try_create_for_physical_range(m_framebuffer_address, page_round_up(framebuffer_size_in_bytes()));
    if (!m_real_framebuffer_vmobject)
        return ENOMEM;

    m_swapped_framebuffer_vmobject = AnonymousVMObject::try_create_with_size(page_round_up(framebuffer_size_in_bytes()), AllocationStrategy::AllocateNow);
    if (!m_swapped_framebuffer_vmobject)
        return ENOMEM;

    m_real_framebuffer_region = MM.allocate_kernel_region_with_vmobject(*m_real_framebuffer_vmobject, page_round_up(framebuffer_size_in_bytes()), "Framebuffer", Region::Access::Read | Region::Access::Write);
    if (!m_real_framebuffer_region)
        return ENOMEM;

    m_swapped_framebuffer_region = MM.allocate_kernel_region_with_vmobject(*m_swapped_framebuffer_vmobject, page_round_up(framebuffer_size_in_bytes()), "Framebuffer Swap (Blank)", Region::Access::Read | Region::Access::Write);
    if (!m_swapped_framebuffer_region)
        return ENOMEM;

    RefPtr<VMObject> chosen_vmobject;
    if (m_graphical_writes_enabled) {
        chosen_vmobject = m_real_framebuffer_vmobject;
    } else {
        chosen_vmobject = m_swapped_framebuffer_vmobject;
    }
    auto result = process.space().allocate_region_with_vmobject(
        range,
        chosen_vmobject.release_nonnull(),
        0,
        "Framebuffer",
        prot,
        shared);
    if (!result.is_error()) {
        m_userspace_framebuffer_region = result.value();
    }
    return result;
}

void FramebufferDevice::deactivate_writes()
{
    ScopedSpinLock lock(m_activation_lock);
    if (!m_userspace_framebuffer_region)
        return;
    memcpy(m_swapped_framebuffer_region->vaddr().as_ptr(), m_real_framebuffer_region->vaddr().as_ptr(), page_round_up(framebuffer_size_in_bytes()));
    auto vmobject = m_swapped_framebuffer_vmobject;
    m_userspace_framebuffer_region->set_vmobject(vmobject.release_nonnull());
    m_userspace_framebuffer_region->remap();
    m_graphical_writes_enabled = false;
}
void FramebufferDevice::activate_writes()
{
    ScopedSpinLock lock(m_activation_lock);
    if (!m_userspace_framebuffer_region || !m_real_framebuffer_vmobject)
        return;

    memcpy(m_real_framebuffer_region->vaddr().as_ptr(), m_swapped_framebuffer_region->vaddr().as_ptr(), page_round_up(framebuffer_size_in_bytes()));
    auto vmobject = m_userspace_real_framebuffer_vmobject;
    m_userspace_framebuffer_region->set_vmobject(vmobject.release_nonnull());
    m_userspace_framebuffer_region->remap();
    m_graphical_writes_enabled = true;
}

String FramebufferDevice::device_name() const
{
    return String::formatted("fb{}", minor());
}

UNMAP_AFTER_INIT void FramebufferDevice::initialize()
{
    m_real_framebuffer_vmobject = AnonymousVMObject::try_create_for_physical_range(m_framebuffer_address, page_round_up(framebuffer_size_in_bytes()));
    VERIFY(m_real_framebuffer_vmobject);
    m_real_framebuffer_region = MM.allocate_kernel_region_with_vmobject(*m_real_framebuffer_vmobject, page_round_up(framebuffer_size_in_bytes()), "Framebuffer", Region::Access::Read | Region::Access::Write);
    VERIFY(m_real_framebuffer_region);
    m_swapped_framebuffer_vmobject = AnonymousVMObject::try_create_with_size(page_round_up(framebuffer_size_in_bytes()), AllocationStrategy::AllocateNow);
    VERIFY(m_swapped_framebuffer_vmobject);
    m_swapped_framebuffer_region = MM.allocate_kernel_region_with_vmobject(*m_swapped_framebuffer_vmobject, page_round_up(framebuffer_size_in_bytes()), "Framebuffer Swap (Blank)", Region::Access::Read | Region::Access::Write);
    VERIFY(m_swapped_framebuffer_region);
}

UNMAP_AFTER_INIT FramebufferDevice::FramebufferDevice(const GraphicsDevice& adapter, size_t output_port_index, PhysicalAddress addr, size_t width, size_t height, size_t pitch)
    : BlockDevice(29, GraphicsManagement::the().allocate_minor_device_number())
    , m_framebuffer_address(addr)
    , m_framebuffer_pitch(pitch)
    , m_framebuffer_width(width)
    , m_framebuffer_height(height)
    , m_output_port_index(output_port_index)
    , m_graphics_adapter(adapter)
{
    VERIFY(!m_framebuffer_address.is_null());
    VERIFY(m_framebuffer_pitch);
    VERIFY(m_framebuffer_width);
    VERIFY(m_framebuffer_height);
    dbgln("Framebuffer {}: address={}, pitch={}, width={}, height={}", minor(), addr, pitch, width, height);
}

size_t FramebufferDevice::framebuffer_size_in_bytes() const
{
    if (m_graphics_adapter->double_framebuffering_capable())
        return m_framebuffer_pitch * m_framebuffer_height * 2;
    return m_framebuffer_pitch * m_framebuffer_height;
}

}