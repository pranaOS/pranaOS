/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/String.h>
#include <kernel/devices/KCOVInstance.h>

namespace Kernel {

KCOVInstance::KCOVInstance(ProcessID pid)
{
    m_pid = pid;
    state = UNUSED;
}

KResult KCOVInstance::buffer_allocate(size_t buffer_size_in_entries)
{
    if (buffer_size_in_entries < 2 || buffer_size_in_entries > KCOV_MAX_ENTRIES)
        return EINVAL;

    this->m_buffer_size_in_entries = buffer_size_in_entries - 1;
    this->m_buffer_size_in_bytes = page_round_up(buffer_size_in_entries * KCOV_ENTRY_SIZE);

    this->vmobject = AnonymousVMObject::try_create_with_size(
        this->m_buffer_size_in_bytes, AllocationStrategy::AllocateNow);
    if (!this->vmobject)
        return ENOMEM;

    this->m_kernel_region = MM.allocate_kernel_region_with_vmobject(
        *this->vmobject, this->m_buffer_size_in_bytes, String::formatted("kcov_{}", this->m_pid),
        Region::Access::Read | Region::Access::Write);
    if (!this->m_kernel_region)
        return ENOMEM;

    this->m_buffer = (u64*)this->m_kernel_region->vaddr().as_ptr();
    if (!this->has_buffer())
        return ENOMEM;

    return KSuccess;
}

void KCOVInstance::buffer_add_pc(u64 pc)
{
    auto idx = (u64)this->m_buffer[0];
    if (idx >= this->m_buffer_size_in_entries) {
        return;
    }

    this->m_buffer[idx + 1] = pc;
    this->m_buffer[0] = idx + 1;
}

}