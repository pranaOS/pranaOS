/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Optional.h>
#include <base/Stream.h>

namespace Base {

class InputBitStream final : public InputStream {
public:
    explicit InputBitStream(InputStream& stream)
        : m_stream(stream)
    {
    }

    size_t read(Bytes bytes) override
    {
        if (has_any_error())
            return 0;

        size_t nread = 0;
        if (bytes.size() >= 1) {
            if (m_next_byte.has_value()) {
                bytes[0] = m_next_byte.value();
                m_next_byte.clear();

                ++nread;
            }
        }

        return nread + m_stream.read(bytes.slice(nread));
    }

    bool read_or_error(Bytes bytes) override
    {
        if (read(bytes) != bytes.size()) {
            set_fatal_error();
            return false;
        }

        return true;
    }

    bool unreliable_eof() const override { return !m_next_byte.has_value() && m_stream.unreliable_eof(); }

};

}

using Base::InputBitStream;
using Base::OutputBitStream;
