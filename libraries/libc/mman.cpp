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
        Syscall::SC_mmap_params params { (uintptr_t)addr, size, alignment, prot, flags, fd, offset, { name, name ? strlen(name) : 0 } };
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
        return prana_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, nullptr);
    }

    /**
     * @param addr 
     * @param size 
     * @param prot 
     * @param flags 
     * @param fd 
     * @param offset 
     * @param name 
     * @return void* 
     */
    void* mmap_with_name(void* addr, size_t size, int prot, int flags, int fd, off_t offset, const char* name)
    {
        return prana_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, name);
    }

    /**
     * @param addr 
     * @param size 
     * @return int 
     */
    int munmap(void* addr, size_t size)
    {
        int rc = syscall(SC_munmap, addr, size);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param addr 
     * @param size 
     * @param prot 
     * @return int 
     */
    int mprotect(void* addr, size_t size, int prot)
    {
        int rc = syscall(SC_mprotect, addr, size, prot);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param addr 
     * @param size 
     * @param name 
     * @return int 
     */
    int set_mmap_name(void* addr, size_t size, const char* name)
    {
        if (!name) {
            errno = EFAULT;
            return -1;
        }

        Syscall::SC_set_mmap_name_params params { addr, size, { name, strlen(name) } };
        int rc = syscall(SC_set_mmap_name, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param address 
     * @param size 
     * @param advice 
     * @return int 
     */
    int madvise(void* address, size_t size, int advice)
    {
        int rc = syscall(SC_madvise, address, size, advice);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param address 
     * @param size 
     * @param inherit 
     * @return int 
     */
    int minherit(void* address, size_t size, int inherit)
    {
        int rc = syscall(SC_minherit, address, size, inherit);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param size 
     * @return void* 
     */
    void* allocate_tls(size_t size)
    {
        int rc = syscall(SC_allocate_tls, size);
        
        if (rc < 0 && -rc < EMAXERRNO) {
            errno = -rc;
            return MAP_FAILED;
        }
        
        return (void*)rc;
    }
}
