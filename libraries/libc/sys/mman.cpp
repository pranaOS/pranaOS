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
     * @param addr 
     * @param size 
     * @param prot 
     * @param flags 
     * @param fd 
     * @param offset 
     * @param name 
     * @return void* 
     */
    void* mmap_with_name(void* addr, size_t size, int prot, int flags, int fd, off_t offset, char const* name)
    {
        return pranaos_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, name);
    }

    /**
     * @param old_address 
     * @param old_size 
     * @param new_size 
     * @param flags 
     * @return void* 
     */
    void* mremap(void* old_address, size_t old_size, size_t new_size, int flags)
    {
        Syscall::SC_mremap_params params { old_address, old_size, new_size, flags };
        ptrdiff_t rc = syscall(SC_mremap, &params);
        if (rc < 0 && rc > -EMAXERRNO) {
            errno = -rc;
            return MAP_FAILED;
        }
        return (void*)rc;
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
     * @brief Set the mmap name object
     * 
     * @param addr 
     * @param size 
     * @param name 
     * @return int 
     */
    int set_mmap_name(void* addr, size_t size, char const* name)
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
     * @param len 
     * @param advice 
     * @return int 
     */
    int posix_madvise(void* address, size_t len, int advice)
    {
        return madvise(address, len, advice);
    }

    /**
     * @param initial_data 
     * @param size 
     * @return void* 
     */
    void* allocate_tls(char const* initial_data, size_t size)
    {
        ptrdiff_t rc = syscall(SC_allocate_tls, initial_data, size);
        if (rc < 0 && rc > -EMAXERRNO) {
            errno = -rc;
            return MAP_FAILED;
        }
        return (void*)rc;
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
    
} // extern "C"
