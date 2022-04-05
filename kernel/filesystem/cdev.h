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
