/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/graphics/virtiogpu/FrameBufferDevice.h>
#include <libc/sys/ioctl_numbers.h>

namespace Kernel::Graphics::VirtIOGPU {
FrameBufferDevice::FrameBufferDevice(GPU& virtio_gpu, ScanoutID scanout)
    : BlockDevice(29, GraphicsManagement::the().allocate_minor_device_number())
    , m_gpu(virtio_gpu)
    , m_scanout(scanout)
{
if (display_info().enabled)
create_framebuffer();
}
FrameBufferDevice::~FrameBufferDevice()
{
}
void FrameBufferDevice::create_framebuffer()
{
    m_framebuffer = nullptr;
    m_framebuffer_sink_vmobject = nullptr;
auto& info = display_info();
    m_buffer_size = calculate_framebuffer_size(info.rect.width, info.rect.height);
    m_framebuffer = MM.allocate_kernel_region(m_buffer_size * 2, String::formatted("VirtGPU FrameBuffer #{}", m_scanout.value()), Region::Access::Read | Region::Access::Write, AllocationStrategy::AllocateNow);
auto write_sink_page = MM.allocate_user_physical_page(MemoryManager::ShouldZeroFill::No).release_nonnull();
auto num_needed_pages = m_framebuffer->vmobject().page_count();
    NonnullRefPtrVector<PhysicalPage> pages;
for (auto i = 0u; i < num_needed_pages; ++i) {
        pages.append(write_sink_page);
    }
    m_framebuffer_sink_vmobject = AnonymousVMObject::try_create_with_physical_pages(pages.span());
    MutexLocker locker(m_gpu.operation_lock());
    m_current_buffer = &buffer_from_index(m_last_set_buffer_index.load());
create_buffer(m_main_buffer, 0, m_buffer_size);
create_buffer(m_back_buffer, m_buffer_size, m_buffer_size);
}
void FrameBufferDevice::create_buffer(Buffer& buffer, size_t framebuffer_offset, size_t framebuffer_size)
{
    buffer.framebuffer_offset = framebuffer_offset;
    buffer.framebuffer_data = m_framebuffer->vaddr().as_ptr() + framebuffer_offset;
auto& info = display_info();
// 1. Create BUFFER using VIRTIO_GPU_CMD_RESOURCE_CREATE_2D
if (buffer.resource_id.value() != 0)
        m_gpu.delete_resource(buffer.resource_id);
    buffer.resource_id = m_gpu.create_2d_resource(info.rect);
// 2. Attach backing storage using  VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING
    m_gpu.ensure_backing_storage(*m_framebuffer, buffer.framebuffer_offset, framebuffer_size, buffer.resource_id);
// 3. Use VIRTIO_GPU_CMD_SET_SCANOUT to link the framebuffer to a display scanout.
if (&buffer == m_current_buffer)
        m_gpu.set_scanout_resource(m_scanout.value(), buffer.resource_id, info.rect);
// 4. Render our test pattern
draw_ntsc_test_pattern(buffer);
// 5. Use VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D to update the host resource from guest memory.
transfer_framebuffer_data_to_host(info.rect, buffer);
// 6. Use VIRTIO_GPU_CMD_RESOURCE_FLUSH to flush the updated resource to the display.
if (&buffer == m_current_buffer)
flush_displayed_image(info.rect, buffer);
// Make sure we constrain the existing dirty rect (if any)
if (buffer.dirty_rect.width != 0 || buffer.dirty_rect.height != 0) {
auto dirty_right = buffer.dirty_rect.x + buffer.dirty_rect.width;
auto dirty_bottom = buffer.dirty_rect.y + buffer.dirty_rect.height;
        buffer.dirty_rect.width = min(dirty_right, info.rect.x + info.rect.width) - buffer.dirty_rect.x;
        buffer.dirty_rect.height = min(dirty_bottom, info.rect.y + info.rect.height) - buffer.dirty_rect.y;
    }
    info.enabled = 1;
}
Protocol::DisplayInfoResponse::Display const& FrameBufferDevice::display_info() const
{
return m_gpu.display_info(m_scanout);
}
Protocol::DisplayInfoResponse::Display& FrameBufferDevice::display_info()
{
return m_gpu.display_info(m_scanout);
}
void FrameBufferDevice::transfer_framebuffer_data_to_host(Protocol::Rect const& rect, Buffer& buffer)
{
    m_gpu.transfer_framebuffer_data_to_host(m_scanout, rect, buffer.resource_id);
}
void FrameBufferDevice::flush_dirty_window(Protocol::Rect const& dirty_rect, Buffer& buffer)
{
    m_gpu.flush_dirty_rectangle(m_scanout, dirty_rect, buffer.resource_id);
}
void FrameBufferDevice::flush_displayed_image(Protocol::Rect const& dirty_rect, Buffer& buffer)
{
    m_gpu.flush_displayed_image(dirty_rect, buffer.resource_id);
}
bool FrameBufferDevice::try_to_set_resolution(size_t width, size_t height)
{
if (width > MAX_VIRTIOGPU_RESOLUTION_WIDTH || height > MAX_VIRTIOGPU_RESOLUTION_HEIGHT)
return false;
auto& info = display_info();
    MutexLocker locker(m_gpu.operation_lock());
    info.rect = {
        .x = 0,
        .y = 0,
        .width = (u32)width,
        .height = (u32)height,
    };
create_framebuffer();
return true;
}
void FrameBufferDevice::set_buffer(int buffer_index)
{
auto& buffer = buffer_index == 0 ? m_main_buffer : m_back_buffer;
    MutexLocker locker(m_gpu.operation_lock());
if (&buffer == m_current_buffer)
return;
    m_current_buffer = &buffer;
    m_gpu.set_scanout_resource(m_scanout.value(), buffer.resource_id, display_info().rect);
    m_gpu.flush_displayed_image(buffer.dirty_rect, buffer.resource_id); // QEMU SDL backend requires this (as per spec)
    buffer.dirty_rect = {};
}
