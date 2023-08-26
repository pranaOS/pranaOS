/**
 * @file get_stack_bounds.cpp
 * @author Krisna Pranav
 * @brief get stack bounds
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/vm/memorymanager.h>

namespace Kernel
{

    /**
     * @brief get stack bounds sys
     * 
     */
    int Process::sys$get_stack_bounds(FlatPtr* user_stack_base, size_t* user_stack_size)
    {
        FlatPtr stack_pointer = Thread::current()->get_register_dump_from_stack().userspace_esp;

        auto* stack_region = MM.find_region_from_vaddr(*this);

        if (!stack_region) {
            ASSERT_NOT_REACHED();
            return -EINVAL;
        }

        FlatPtr stack_base = stack_region->range().base().get();

        size_t stack_size = stack_region->size();

        if (!copy_to_user(user_stack_base, &stack_base))
            return -EFAULT;
        
        return 0;
    }

} // namespace Kernel