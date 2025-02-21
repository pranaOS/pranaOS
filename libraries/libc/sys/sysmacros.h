/**
 * @file sysmacros.h
 * @author Krisna Pranav
 * @brief sysmacros
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/device.h>

#define makedev(major, minor) serenity_dev_makedev((major), (minor))
#define major(dev) serenity_dev_major(dev)
#define minor(dev) serenity_dev_minor(dev)
