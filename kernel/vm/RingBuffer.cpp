/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/UserOrKernelBuffer.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/RingBuffer.h>


namespace Kernel
{

RingBuffer::RingBuffer(String region_name, size_t capacity)
    : m_region(MM.allocate_contiguous_kernel_region(page_round_up(capacity), move(region_name), Region::Access::Read | Region::Access::Write))
    , m_capacity_in_bytes(capacity)
{
}

void RingBuffer::reclaim_spec(PhysicalAddress chunk_start, size_t chunk_size)
{
    VERIFY(start_of_used() == chunk_start);
    VERIFY(m_num_used_bytes >= chunk_size);
    m_num_used_bytes -= chunk_size;
    m_start_of_used += chunk_size;
}

PhyiscalAddress RingBuffer::start_of_used() const
{
    size_t start = m_start_of_used % m_capacity_in_bytes;
    return m_region->physical_page(start / PAGE_SIZE)->paddr().offset(start % PAGE_SIZE);
}

}
