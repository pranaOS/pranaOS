/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Span.h>
#include <base/Types.h>
#include <libcrypto/checksum/Adler32.h>

namespace Crypto::Checksum {

void Adler32::update(ReadonlyBytes data)
{
    for (size_t i = 0; i < data.size(); i++) {
        m_state_a = (m_state_a + data.at(i)) % 65521;
        m_state_b = (m_state_b + m_state_a) % 65521;
    }
};

u32 Adler32::digest()
{
    return (m_state_b << 16) | m_state_a;
}

}
