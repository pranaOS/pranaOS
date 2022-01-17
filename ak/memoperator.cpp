/*
 * Copyright (c) 2021-2022, Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
*/


#include "memoperator.h"

using namespace pranaOS::ak;

void* MemOperator::memmove(void* dstptr, const void* srcptr, uint32_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
    
	if (dst < src) {
		for (uint32_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (uint32_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}

	return dstptr;
}

int MemOperator::memcmp(const void* aptr, const void* bptr, uint32_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;

	for (uint32_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}

	return 0;
}

void* MemOperator::memset(void* bufptr, char value, uint32_t size) {
    unsigned char* buf = (unsigned char*) bufptr;
    
	for (uint32_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;

	return bufptr;
}

void* MemOperator::memcpy(void* dstptr, const void* srcptr, uint32_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;

	for (uint32_t i = 0; i < size; i++)
		dst[i] = src[i];

	return dstptr;
}