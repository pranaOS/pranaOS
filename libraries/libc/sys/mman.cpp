/**
 * @file mman.cpp
 * @author Krisna Pranav
 * @brief mman
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/format.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <syscall.h>

extern "C" {

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
void* pranaos_mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset, size_t alignment, char const* name)
{
    Syscall::SC_mmap_params params { addr, size, alignment, prot, flags, fd, offset, { name, name ? strlen(name) : 0 } };
    ptrdiff_t rc = syscall(SC_mmap, &params);
    if (rc < 0 && rc > -EMAXERRNO) {
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
    return pranaos_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, nullptr);
}

/**
 * @return int 
 */
int mlock(void const*, size_t)
{
    dbgln("FIXME: Implement mlock()");
    return 0;
}

/**
 * @return int 
 */
int munlock(void const*, size_t)
{
    dbgln("FIXME: Implement munlock()");
    return 0;
}

/**
 * @param address 
 * @param size 
 * @param flags 
 * @return int 
 */
int msync(void* address, size_t size, int flags)
{
    int rc = syscall(SC_msync, address, size, flags);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}