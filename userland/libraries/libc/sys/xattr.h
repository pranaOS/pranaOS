/**
 * @file xattr.h
 * @author Krisna Pranav
 * @brief xattr
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/types.h>

__BEGIN_DECLS

/**
 * @param path 
 * @param name 
 * @param value 
 * @param size 
 * @return ssize_t 
 */
ssize_t getxattr(char const* path, char const* name, void* value, size_t size);

/**
 * @param path 
 * @param name 
 * @param value 
 * @param size 
 * @return ssize_t 
 */
ssize_t lgetxattr(char const* path, char const* name, void* value, size_t size);

/**
 * @param fd 
 * @param name 
 * @param value 
 * @param size 
 * @return ssize_t 
 */
ssize_t fgetxattr(int fd, char const* name, void* value, size_t size);

/**
 * @param path 
 * @param name 
 * @param value 
 * @param size 
 * @param flags 
 * @return int 
 */
int setxattr(char const* path, char const* name, void const* value, size_t size, int flags);

/**
 * @param path 
 * @param name 
 * @param value 
 * @param size 
 * @param flags 
 * @return int 
 */
int lsetxattr(char const* path, char const* name, void const* value, size_t size, int flags);

/**
 * @param fd 
 * @param name 
 * @param value 
 * @param size 
 * @param flags 
 * @return int 
 */
int fsetxattr(int fd, char const* name, void const* value, size_t size, int flags);

/**
 * @param path 
 * @param list 
 * @param size 
 * @return ssize_t 
 */
ssize_t listxattr(char const* path, char* list, size_t size);

/**
 * @param path 
 * @param list 
 * @param size 
 * @return ssize_t 
 */
ssize_t llistxattr(char const* path, char* list, size_t size);

/**
 * @param fd 
 * @param list 
 * @param size 
 * @return ssize_t 
 */
ssize_t flistxattr(int fd, char* list, size_t size);

__END_DECLS
