/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include "vfs.h"

#define MINORBITS 20
#define MINORMASK ((1U << MINORBITS) - 1)

#define MAJOR(dev) ((unsigned int)((dev) >> MINORBITS))
#define MINOR(dev) ((unsigned int)((dev)&MINORMASK))
#define MKDEV(ma, mi) (((ma) << MINORBITS) | (mi))

#define PATH_DEV "/dev/"
#define SPECNAMELEN 255

/**
 * @brief c devices 
 * 
 */
struct cdevices {
    const char *name;
    uint32_t major;
    uint32_t baseminor;
    int32_t minorct;

    dev_t dev;
    struct list_head sibling;
    struct vfs_file_operations *f_ops;
};

/**
 * @brief declare chardev
 * 
 */
#define DECLARE_CHARDEV(_name, _major, _baseminor, _minorct, _ops) \
	{                                                             \
		.name = _name,                                            \
		.major = _major,                                          \
		.baseminor = _baseminor,                                  \
		.minorct = _minorct,                                      \
		.dev = MKDEV(_major, _baseminor),                         \
		.f_ops = _ops,                                            \
	}


/**
 * @brief alloc chardev
 * 
 * @param name 
 * @param major 
 * @param minor 
 * @param minorct 
 * @param ops 
 * @return struct cdevices* 
 */
struct cdevices *alloc_chardev(const char *name, uint32_t major, uint32_t minor, int32_t minorct, struct vfs_file_operations *ops);

/**
 * @brief register chardev
 * 
 * @param cdev 
 * @return int 
 */
int register_chardev(struct cdevices *cdev);

/**
 * @brief unregister chardev
 * 
 * @param dev 
 * @return int 
 */
int unregister_chardev(dev_t dev);

/**
 * @brief dev_char_fops
 * 
 */
extern struct vfs_file_operations def_char_fops;

/**
 * 
 * @brief char dev init
 * 
 */
void chardev_init();
