/**
* @file tss.h
* @author Krisna Pranav
* @brief TSS
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/types.h"

typedef struct TSS {
    uint16_t link;
    uint16_t link_high;

} __attribute__((packed)) TSS;