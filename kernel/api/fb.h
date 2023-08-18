/**
 * @file fb.h
 * @author Krisna Pranav
 * @brief FB
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <mods/platform.h>
// #include <libraries/libc/sys/ioctl_numbers.h>
#include <ioctl_numbers.h>

__BEGIN_DECLS

/**
 * @param fd 
 * @param out 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE int fb_get_size_in_bytes(int fd, size_t* out) {
    return ioctl(fd, FB_IOCTL_GET_SIZE_IN_BYTES, out);
}

/**
 * @param fd 
 * @param info 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE int fb_get_resolution(int fd, FBResolution* info) {
    return ioctl(fd, FB_IOCTL_GET_RESOLUTION, info);
}

/**
 * @param fd 
 * @param info 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE int fb_set_resolution(int fd, FBResolution* info) {
    return ioctl(fd, FB_IOCTL_SET_RESOLUTION, info);
}

/**
 * @param fd 
 * @param index 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE int fb_get_buffer(int fd, int* index) {
    return ioctl(fd, FB_IOCTL_GET_BUFFER, index);
}

/**
 * @param fd 
 * @param index 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE int fb_set_buffer(int fd, int index) {
    return ioctl(fd, FB_IOCTL_SET_BUFFER, index);
}

__END_DECLS