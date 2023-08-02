/**
 * @file mman.cpp
 * @author Krisna Pranav
 * @brief mman
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno.h>
#include <mman.h>
#include <stdio.h>
#include <string.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @param addr 
     * @param size 
     * @param prot 
     * @param flags 
     * @param fd 
     * @param offset 
     * @param alignment 
     * @param name 
     * @return void* 
     */
    void* prana_mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset, size_t alignment, const char* name)
    {
        Syscall::SC_mmap_params params { (uintptr_t) addr, size, alignment, prot, fd };
        ssize_t rc = syscall(SC_mmap, &params);

        if (rc < 0 && -rc < EMAXERRNO) {
            errno = -rc;
            return MAP_FAILED;
        }
        
        return (void*)rc;
    }

    /**
     * @param addr 
     * @param size 
     * @param prot 
     * @param flags 
     * @param fd 
     * @param offset 
     * @return void* 
     */
    void* mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset)
    {
        prana_mmap(addr, size, prot, flags, fd, offset);
    }
}