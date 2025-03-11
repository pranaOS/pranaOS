/**
 * @file auxv.cpp
 * @author Krisna Pranav
 * @brief auxv
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <sys/auxv.h>
#include <sys/internals.h>

extern "C" {
    /**
     * @param type 
     * @return long 
     */
    long getauxval(long type)
    {
        errno = 0;

        auxv_t* auxvp = (auxv_t*)__auxiliary_vector;
        for (; auxvp->a_type != AT_NULL; ++auxvp) {
            if (auxvp->a_type == type)
                return auxvp->a_un.a_val;
        }
        errno = ENOENT;
        return 0;
    }
} // extern "C"
