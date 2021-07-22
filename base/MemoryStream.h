/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/LEB128.h>
#include <base/MemMem.h>
#include <base/Stream.h>
#include <base/Vector.h>

namespace Base {

class InputMemoryStream final : public InputStream {
public:
        explicit InputMemoryStream(ReadonlyBytes bytes)
            : m_bytes(bytes)
        {
        }

        bool unreliable_eof() const override { return eof(); }
        bool eof() const { return m_offset >= bytes.size(); }

        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            const auto count = min(bytes.size(), remaining());
            __builtin_memcpy(bytes.data(), m_bytes.data() + m_offset, count);
            m_offset += count;
            return count;
        }

        bool read_or_error(Bytes bytes) override
        {
            if (remaining() < bytes.size()) {
                set_recoverable_error();
                return false;
            }

            __builtin_memcpy(bytes.data(), m_bytes.data() + m_offset, bytes.size());
            m_offset += bytes.size();
            return true;
        }

        bool discard_or_error(size_t count) override
        {
            if (remaining() < count) {
                set_recoverable_error();
                return false;
            }

            m_offset += count;
            return true;
        }

};

}