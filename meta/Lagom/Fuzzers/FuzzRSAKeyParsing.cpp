/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libcrypto/PK/RSA.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    ByteBuffer rsa_data = ByteBuffer::copy(data, size);
    Crypto::PK::RSA::parse_rsa_key(rsa_data);
    return 0;
}