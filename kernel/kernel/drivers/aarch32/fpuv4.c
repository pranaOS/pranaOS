/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/aarch32/fpuv4.h>
#include <libkernel/libkernel.h>
#include <platform/aarch32/registers.h>

void fpuv4_install()
{
    write_cpacr(read_cpacr() | ((0b1111) << 20));
    write_nsacr(read_nsacr() | ((0b11) << 10));
    write_hcptr(read_hcptr() | ((0b11) << 10));
    fpu_enable();
    fpu_make_unavail();
}

void fpu_init_state(fpu_state_t* new_fpu_state)
{
    memset(new_fpu_state, 0, sizeof(fpu_state_t));
}