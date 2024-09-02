/**
 * @file device.h
 * @author Krisna Pranav
 * @brief DEVICE
 * @version 6.0
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/platform.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * @param major 
 * @param minor 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE dev_t pranaos_dev_makedev(unsigned major, unsigned minor)
{
    return (minor & 0xffu) | (major << 8u) | ((minor & ~0xffu) << 12u);
}

/**
 * @param dev 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE unsigned int pranaos_dev_major(dev_t dev)
{
    return (dev & 0xfff00u) >> 8u;
}

/**
 * @param dev 
 * @return ALWAYS_INLINE 
 */
ALWAYS_INLINE unsigned int pranaos_dev_minor(dev_t dev)
{
    return (dev & 0xffu) | ((dev >> 12u) & 0xfff00u);
}

__END_DECLS
