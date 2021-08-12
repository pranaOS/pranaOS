/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Atomic.h>
#include <kernel/virtio/VirtIOQueue.h>

namespace Kernel {

VirtIOQueue::VirtIOQueue(u16 queue_size, u16 notify_offset)
    : m_queue_size(queue_size)
    , m_notify_offset(notify_offset)
    , m_free_buffers(queue_size)
{
    size_t size_of_descriptors = sizeof(VirtIOQueueDescriptor) * queue_size;
    size_t size_of_driver = sizeof(VirtIOQueueDriver) + queue_size * sizeof(u16);
    size_t size_of_device = sizeof(VirtIOQueueDevice) + queue_size * sizeof(VirtIOQueueDeviceItem);
    auto queue_region_size = Memory::page_round_up(size_of_descriptors + size_of_driver + size_of_device);
    if (queue_region_size <= PAGE_SIZE)
        m_queue_region = MM.allocate_kernel_region(queue_region_size, "VirtIO Queue", Memory::Region::Access::ReadWrite);
    else
        m_queue_region = MM.allocate_contiguous_kernel_region(queue_region_size, "VirtIO Queue", Memory::Region::Access::ReadWrite);
    VERIFY(m_queue_region);
    // TODO: ensure alignment!!!
    u8* ptr = m_queue_region->vaddr().as_ptr();
    memset(ptr, 0, m_queue_region->size());
    m_descriptors = adopt_own_if_nonnull(reinterpret_cast<VirtIOQueueDescriptor*>(ptr));
    m_driver = adopt_own_if_nonnull(reinterpret_cast<VirtIOQueueDriver*>(ptr + size_of_descriptors));
    m_device = adopt_own_if_nonnull(reinterpret_cast<VirtIOQueueDevice*>(ptr + size_of_descriptors + size_of_driver));

    for (auto i = 0; i + 1 < queue_size; i++) {
        m_descriptors[i].next = i + 1; 
    }

    enable_interrupts();
}

VirtIOQueue::~VirtIOQueue()
{
}

void VirtIOQueue::enable_interrupts()
{
    ScopedSpinLock lock(m_lock);
    m_driver->flags = 0;
}

void VirtIOQueue::disable_interrupts()
{
    ScopedSpinLock lock(m_lock);
    m_driver->flags = 1;
}

bool VirtIOQueue::new_data_available() const
{
    const auto index = Base::atomic_load(&m_device->index, Base::MemoryOrder::memory_order_relaxed);
    const auto used_tail = Base::atomic_load(&m_used_tail, Base::MemoryOrder::memory_order_relaxed);
    return index != used_tail;
}

VirtIOQueueChain VirtIOQueue::pop_used_buffer_chain(size_t& used)
{
    VERIFY(m_lock.is_locked());
    if (!new_data_available()) {
        used = 0;
        return VirtIOQueueChain(*this);
    }

    full_memory_barrier();

    used = m_device->rings[m_used_tail % m_queue_size].length;

    auto descriptor_index = m_device->rings[m_used_tail % m_queue_size].index;
    size_t length_of_chain = 1;
    auto last_index = descriptor_index;
    while (m_descriptors[last_index].flags & VIRTQ_DESC_F_NEXT) {
        ++length_of_chain;
        last_index = m_descriptors[last_index].next;
    }

    m_used_tail++;

    return VirtIOQueueChain(*this, descriptor_index, last_index, length_of_chain);
}

void VirtIOQueue::discard_used_buffers()
{
    VERIFY(m_lock.is_locked());
    size_t used;
    for (auto buffer = pop_used_buffer_chain(used); !buffer.is_empty(); buffer = pop_used_buffer_chain(used)) {
        buffer.release_buffer_slots_to_queue();
    }
}

void VirtIOQueue::reclaim_buffer_chain(u16 chain_start_index, u16 chain_end_index, size_t length_of_chain)
{
    VERIFY(m_lock.is_locked());
    m_descriptors[chain_end_index].next = m_free_head;
    m_free_head = chain_start_index;
    m_free_buffers += length_of_chain;
}

bool VirtIOQueue::has_free_slots() const
{
    const auto free_buffers = Base::atomic_load(&m_free_buffers, Base::MemoryOrder::memory_order_relaxed);
    return free_buffers > 0;
}

Optional<u16> VirtIOQueue::take_free_slot()
{
    VERIFY(m_lock.is_locked());
    if (has_free_slots()) {
        auto descriptor_index = m_free_head;
        m_free_head = m_descriptors[descriptor_index].next;
        --m_free_buffers;
        return descriptor_index;
    } else {
        return {};
    }
}

bool VirtIOQueue::should_notify() const
{
    VERIFY(m_lock.is_locked());
    auto device_flags = m_device->flags;
    return !(device_flags & VIRTQ_USED_F_NO_NOTIFY);
}

bool VirtIOQueueChain::add_buffer_to_chain(PhysicalAddress buffer_start, size_t buffer_length, BufferType buffer_type)
{
    VERIFY(m_queue.lock().is_locked());


    VERIFY(buffer_type == BufferType::DeviceWritable || !m_chain_has_writable_pages);
    m_chain_has_writable_pages |= (buffer_type == BufferType::DeviceWritable);

    auto descriptor_index = m_queue.take_free_slot();
    if (!descriptor_index.has_value())
        return false;

    if (!m_start_of_chain_index.has_value()) {

        m_start_of_chain_index = descriptor_index.value();
    } else {

        m_queue.m_descriptors[m_end_of_chain_index.value()].flags |= VIRTQ_DESC_F_NEXT;
        m_queue.m_descriptors[m_end_of_chain_index.value()].next = descriptor_index.value();
    }


    m_end_of_chain_index = descriptor_index.value();
    ++m_chain_length;

    VERIFY(buffer_length <= NumericLimits<size_t>::max());
    m_queue.m_descriptors[descriptor_index.value()].address = static_cast<u64>(buffer_start.get());
    m_queue.m_descriptors[descriptor_index.value()].flags = static_cast<u16>(buffer_type);
    m_queue.m_descriptors[descriptor_index.value()].length = static_cast<u32>(buffer_length);

    return true;
}

void VirtIOQueueChain::submit_to_queue()
{
    VERIFY(m_queue.lock().is_locked());
    VERIFY(m_start_of_chain_index.has_value());

    auto next_index = m_queue.m_driver_index_shadow % m_queue.m_queue_size;
    m_queue.m_driver->rings[next_index] = m_start_of_chain_index.value();
    m_queue.m_driver_index_shadow++;
    full_memory_barrier();
    m_queue.m_driver->index = m_queue.m_driver_index_shadow;

    m_start_of_chain_index = m_end_of_chain_index = {};
    m_chain_has_writable_pages = false;
    m_chain_length = 0;
}

void VirtIOQueueChain::release_buffer_slots_to_queue()
{
    VERIFY(m_queue.lock().is_locked());
    if (m_start_of_chain_index.has_value()) {
        // Add the currently stored chain back to the queue's free pool
        m_queue.reclaim_buffer_chain(m_start_of_chain_index.value(), m_end_of_chain_index.value(), m_chain_length);
        // Reset internal chain state
        m_start_of_chain_index = m_end_of_chain_index = {};
        m_chain_has_writable_pages = false;
        m_chain_length = 0;
    }
}

}
