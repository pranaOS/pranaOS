/**
 * @file wchar_size.h
 * @author Krisna Pranav
 * @brief wchar size
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define WCHAR_MAX __WCHAR_MAX__
#ifdef __WCHAR_MIN__
#    define WCHAR_MIN __WCHAR_MIN__
#else
#    define WCHAR_MIN (-WCHAR_MAX - 1)
#endif
