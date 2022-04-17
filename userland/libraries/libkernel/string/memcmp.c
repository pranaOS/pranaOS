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

/**
 * @brief memcmp common alignment
 * 
 * @param srcp1 
 * @param srcp2 
 * @param len 
 * @return int 
 */
static int memcmp_common_alignment(long int srcp1, long int srcp2, size_t len) {
	op_t a0, a1;
	op_t b0, b1;

	switch (len % 4) {
	default: 
	case 2:
		a0 = ((op_t *)srcp1)[0];
		b0 = ((op_t *)srcp2)[0];
		srcp1 -= 2 * OPSIZ;
		srcp2 -= 2 * OPSIZ;
		len += 2;
		goto do1;
	case 3:
		a1 = ((op_t *)srcp1)[0];
		b1 = ((op_t *)srcp2)[0];
		srcp1 -= OPSIZ;
		srcp2 -= OPSIZ;
		len += 1;
		goto do2;
	case 0:
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			return 0;
		a0 = ((op_t *)srcp1)[0];
		b0 = ((op_t *)srcp2)[0];
		goto do3;
	case 1:
		a1 = ((op_t *)srcp1)[0];
		b1 = ((op_t *)srcp2)[0];
		srcp1 += OPSIZ;
		srcp2 += OPSIZ;
		len -= 1;
		if (OP_T_THRES <= 3 * OPSIZ && len == 0)
			goto do0;
	}

	do
	{
		a0 = ((op_t *)srcp1)[0];
		b0 = ((op_t *)srcp2)[0];
		if (a1 != b1)
			return CMP_LT_OR_GT(a1, b1);

	do3:
		a1 = ((op_t *)srcp1)[1];
		b1 = ((op_t *)srcp2)[1];
		if (a0 != b0)
			return CMP_LT_OR_GT(a0, b0);

	do2:
		a0 = ((op_t *)srcp1)[2];
		b0 = ((op_t *)srcp2)[2];
		if (a1 != b1)
			return CMP_LT_OR_GT(a1, b1);

	do1:
		a1 = ((op_t *)srcp1)[3];
		b1 = ((op_t *)srcp2)[3];
		if (a0 != b0)
			return CMP_LT_OR_GT(a0, b0);

		srcp1 += 4 * OPSIZ;
		srcp2 += 4 * OPSIZ;
		len -= 4;
	} while (len != 0);


do0:
	if (a1 != b1)
		return CMP_LT_OR_GT(a1, b1);
	return 0;
}