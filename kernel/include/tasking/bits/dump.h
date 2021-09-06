/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>
#include <tasking/proc.h>

typedef int (*dump_saver_t)(const char*);
typedef ssize_t (*sym_resolver_t)(void* symtab, size_t syms_n, uint32_t tp);

struct dump_data {
    proc_t* p;
    uint32_t entry_point;
    void* syms;
    size_t symsn;
    char* strs;
    dump_saver_t writer;
    sym_resolver_t sym_resolver;
};
typedef struct dump_data dump_data_t;