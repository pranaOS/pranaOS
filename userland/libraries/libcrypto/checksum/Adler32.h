/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Span.h>
#include <base/Types.h>
#include <libcrypto/checksum/ChecksumFunction.h>

namespace Crypto::Checksum {

class Adler32 : public ChecksumFunction<u32> {
public:
    Adler32() { }
    Adler32(ReadonlyBytes data)
    {
        update(data);
    }

    Adler32(u32 initial_a, u32 initial_b, ReadonlyBytes data)
        : m_state_a(initial_a)
        , m_state_b(initial_b)
    {
        update(data);
    }

    void update(ReadonlyBytes data);
    u32 digest();

private:
    u32 m_state_a { 1 };
    u32 m_state_b { 0 };
};

}
