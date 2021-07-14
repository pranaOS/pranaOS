/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/Prelude.h>

#define ELF_MAG0 0x7F
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'

#define ELF_IDENT_COUNT 16
#define ELF_IDENT_MAG0 0
#define ELF_IDENT_MAG1 1
#define ELF_IDENT_MAG2 2
#define ELF_IDENT_MAG3 3

#define ELF_IDENT_CLASS 4
#define ELF_IDENT_CLASS_NONE 0
#define ELF_IDENT_CLASS_32 1
#define ELF_IDENT_CLASS_64 2

#define ELF_IDENT_DATA 5
#define ELF_IDENT_DATA_NONE 0
#define ELF_IDENT_DATA_LSB 1
#define ELF_IDENT_DATA_MSB 2

#define ELF_IDENT_VERSION 6
#define ELF_IDENT_OSABI 7
#define ELF_IDENT_ABIVERSION 8
#define ELF_IDENT_PAD 9

#define ELF_ETYPE_NONE 0
#define ELF_ETYPE_REL 1
#define ELF_ETYPE_EXEC 2
#define ELF_ETYPE_DYN 3
#define ELF_ETYPE_CORE 4

#define ELF_MACHINE_NONE 0
#define ELF_MACHINE_SPARC 2
#define ELF_MACHINE_386 3
#define ELF_MACHINE_SPARC32PLUS 18
#define ELF_MACHINE_SPARCV9 43
#define ELF_MACHINE_AMD64 62