/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/memory/Region.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$get_stack_bounds(Userspace<FlatPtr*> user_stack_base, Userspace<size_t*> user_stack_size)
{
    VERIFY_NO_PROCESS_BIG_LOCK(this);
    auto& regs = Thread::current()->get_register_dump_from_stack();
    FlatPtr stack_pointer = regs.userspace_sp();
    auto* stack_region = address_space().find_region_containing(Memory::VirtualRange { VirtualAddress(stack_pointer), 1 });

    VERIFY(stack_region);

    FlatPtr stack_base = stack_region->range().base().get();
    size_t stack_size = stack_region->size();
    if (!copy_to_user(user_stack_base, &stack_base))
        return EFAULT;
    if (!copy_to_user(user_stack_size, &stack_size))
        return EFAULT;
    return 0;
}

}
