/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libkernel/string.h>

#define OP_T_THRES 16

typedef unsigned char byte;

#define MERGE(w0, sh_1, w1, sh_2) (((w0) >> (sh_1)) | ((w1) << (sh_2)))

#define CMP_LT_OR_GT(a, b) memcmp_bytes((a), (b))


/**
 * @brief memcmpy bytes
 * 
 * @param a 
 * @param b 
 * @return int 
 */
static int memcmpy_bytes(op_t a, op_t b) {
    long int srcp1 = (long int)&a;
    long int srcp2 = (long int)&b;
    op_t a0, b0;

    do {
        a0 = ((byte *)srcp1)[0];
        b0 = ((byte *)srcp2)[0];
        srcp1 += 1;
        srcp2 += 1;
    } while (a0 == b0);
    return a0 - b0;
}