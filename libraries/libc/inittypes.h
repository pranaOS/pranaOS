/**
 * @file inittypes.h
 * @author Krisna Pranav
 * @brief init types
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/stdint.h>

#define PRId8 "d"
#define PRId16 "d"
#define PRId32 "d"
#define PRId64 "lld"
#define PRIi8 "d"
#define PRIi16 "d"
#define PRIi32 "d"
#define PRIi64 "lld"
#define PRIu8 "u"
#define PRIo8 "o"
#define PRIo16 "o"
#define PRIo32 "o"
#define PRIo64 "llo"
#define PRIu16 "u"
#define PRIu32 "u"
#define PRIu64 "llu"
#define PRIx8 "b"
#define PRIx16 "w"
#define PRIx32 "x"
#define PRIX32 "X"
#define PRIx64 "llx"
#define PRIX64 "llX"

#define __PRI64_PREFIX "ll"
#define __PRIPTR_PREFIX

#define PRIdPTR __PRIPTR_PREFIX "d"
#define PRIiPTR __PRIPTR_PREFIX "i"
#define PRIXPTR __PRIPTR_PREFIX "X"

#define PRIdMAX __PRI64_PREFIX "d"
#define PRIoMAX __PRI64_PREFIX "o"
#define PRIuMAX __PRI64_PREFIX "u"

#define SCNdMAX __PRI64_PREFIX "d"
#define SCNoMAX __PRI64_PREFIX "o"
#define SCNuMAX __PRI64_PREFIX "u"
