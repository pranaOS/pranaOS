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

    bool discard_or_error(size_t count) override
    {
        if (count >= 1) {
            if (m_next_byte.has_value()) {
                m_next_byte.clear();
                --count;
            }
        }

        return m_stream.discard_or_error(count);
    }

    u64 read_bits(size_t count)
    {
        u64 result = 0;

        size_t nread = 0;
        while (nread < count) {
            if (m_stream.has_any_error()) {
                set_fatal_error();
                return 0;
            }

            if (m_next_byte.has_value()) {
                const auto bit = (m_next_byte.value() >> m_bit_offset) & 1;
                result |= bit << nread;
                ++nread;

                if (m_bit_offset++ == 7)
                    m_next_byte.clear();
            } else {
                m_stream >> m_next_byte;
                m_bit_offset = 0;
            }
        }

        return result;
    }

    u64 read_bits_big_endian(size_t count)
    {
        u64 result = 0;

        size_t nread = 0;
        while (nread < count) {
            if (m_stream.has_any_error()) {
                set_fatal_error();
                return 0;
            }

            if (m_next_byte.has_value()) {
                // read an entire byte
                if (((count - nread) >= 8) && m_bit_offset == 0) {
                    // shift existing bytes over
                    result <<= 8;
                    result |= m_next_byte.value();
                    nread += 8;
                    m_next_byte.clear();
                } else {
                    const auto bit = (m_next_byte.value() >> (7 - m_bit_offset)) & 1;
                    result <<= 1;
                    result |= bit;
                    ++nread;
                    if (m_bit_offset++ == 7)
                        m_next_byte.clear();
                }
            } else {
                m_stream >> m_next_byte;
                m_bit_offset = 0;
            }
        }

        return result;
    }

    bool read_bit() { return static_cast<bool>(read_bits(1)); }

    bool read_bit_big_endian() { return static_cast<bool>(read_bits_big_endian(1)); }

    void align_to_byte_boundary()
    {
        if (m_next_byte.has_value())
            m_next_byte.clear();
    }

    bool handle_any_error() override
    {
        bool handled_errors = m_stream.handle_any_error();
        return Stream::handle_any_error() || handled_errors;
    }

private:
    Optional<u8> m_next_byte;
    size_t m_bit_offset { 0 };
    InputStream& m_stream;
};


}

using Base::InputBitStream;
using Base::OutputBitStream;
