/**
 * @file kcov.h
 * @author Krisna Pranav
 * @brief kcov
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>


typedef volatile u64 kcov_pc_t;

#define KCOV_ENTRY_SIZE sizeof(kcov_pc_t)
