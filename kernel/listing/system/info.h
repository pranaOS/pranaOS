/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>

namespace Kernel {

    /**
     * @brief SIBIOS struct 
     * 
     */
    struct SIBIOS
    {
        char* vendor = "N/A";
        char* version = "N/A";
        char* releaseDate = "N/A";
    };

    /**
     * @brief SISYSTEM
     * 
     */
    struct SISYSTEM
    {
        char* manufacturer = "N/A";
        char* product = "N/A";
        char* version = "N/A";
        char* serial = "N/A";
        char* family = "N/A";  
    };

    /**
     * @brief SIENCLOSURE
     * 
     */
    struct SIENCLOSURE
    {
        char* manufacturer = "N/A";
        char* version = "N/A";
        char* serial = "N/A";
        char* sku = "N/A";
    };

    /**
     * @brief SIPROCESSOR
     * 
     */
    struct SIPROCESSOR
    {
        char* socket = "N/A";
        char* manufacturer = "N/A";
        char* version = "N/A";
    }; 
}