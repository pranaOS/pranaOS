/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>

namespace Kernel {

    /**
     * @brief si BIOS info
     * 
     */
    struct siBIOS 
    {
        char* vendor = "N/A";
        char* version = "N/A";
        char* releaseDate = "N/A";
    };

    /**
     * @brief si system 
     * 
     */
    struct siSystem 
    {
        char* manufacturer = "N/A";
        char* product = "N/A";
        char* version = "N/A";
        char* serial = "N/A";
        char* family = "N/A";
    };
}