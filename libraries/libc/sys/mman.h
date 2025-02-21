/**
 * @file mman.h
 * @author Krisna Pranav
 * @brief MMAN
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/mman.h>

__BEGIN_DECLS

/**
 * @param addr 
 * @param prot 
 * @param flags 
 * @param fd 
 * @return void* 
 */
void* mmap(void* addr, size_t, int prot, int flags, int fd, off_t);

/**
 * @param addr 
 * @param prot 
 * @param flags 
 * @param fd 
 * @param name 
 * @return void* 
 */
void* mmap_with_name(void* addr, size_t, int prot, int flags, int fd, off_t, char const* name);

/**
 * @param addr 
 * @param prot 
 * @param flags 
 * @param fd 
 * @param alignment 
 * @param name 
 * @return void* 
 */
void* pranaos_mmap(void* addr, size_t, int prot, int flags, int fd, off_t, size_t alignment, char const* name);

/**
 * @param old_address 
 * @param old_size 
 * @param new_size 
 * @param flags 
 * @return void* 
 */
void* mremap(void* old_address, size_t old_size, size_t new_size, int flags);

/**
 * @return int 
 */
int munmap(void*, size_t);

/**
 * @param prot 
 * @return int 
 */
int mprotect(void*, size_t, int prot);

/**
 * @brief Set the mmap name object
 * 
 * @return int 
 */
int set_mmap_name(void*, size_t, char const*);

/**
 * @param advice 
 * @return int 
 */
int madvise(void*, size_t, int advice);

/**
 * @param advice 
 * @return int 
 */
int posix_madvise(void*, size_t, int advice);

/**
 * @param initial_data 
 * @return void* 
 */
void* allocate_tls(char const* initial_data, size_t);

/**
 * @return int 
 */
int mlock(void const*, size_t);

/**
 * @return int 
 */
int munlock(void const*, size_t);

/**
 * @param flags 
 * @return int 
 */
int msync(void*, size_t, int flags);

__END_DECLS
