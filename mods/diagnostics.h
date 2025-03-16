/**
 * @file diagnostics.h
 * @author Krisna Pranav
 * @brief diagnostics
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define _MODS_PRAGMA(x) _Pragma(#x)
#define MODS_PRAGMA(x) _MODS_PRAGMA(x)

#define MODS_IGNORE_DIAGNOSTIC(name, statement) \
    MODS_PRAGMA(GCC diagnostic push);           \
    MODS_PRAGMA(GCC diagnostic ignored name);   \
    statement;                                \
    MODS_PRAGMA(GCC diagnostic pop);
