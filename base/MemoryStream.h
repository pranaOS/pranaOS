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
    bool eof() const { return m_offset >= m_bytes.size(); }

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

    void seek(size_t offset)
    {
        VERIFY(offset < m_bytes.size());
        m_offset = offset;
    }

    u8 peek_or_error() const
    {
        if (remaining() == 0) {
            set_recoverable_error();
            return 0;
        }

        return m_bytes[m_offset];
    }

    bool read_LEB128_unsigned(size_t& result) { return LEB128::read_unsigned(*this, result); }

    bool read_LEB128_signed(ssize_t& result) { return LEB128::read_signed(*this, result); }

    ReadonlyBytes bytes() const { return m_bytes; }
    size_t offset() const { return m_offset; }
    size_t remaining() const { return m_bytes.size() - m_offset; }

private:
    ReadonlyBytes m_bytes;
    size_t m_offset { 0 };
};

class OutputMemoryStream final : public OutputStream {
public:
    explicit OutputMemoryStream(Bytes bytes)
        : m_bytes(bytes)
    {
    }

    size_t write(ReadonlyBytes bytes) override
    {
        const auto nwritten = bytes.copy_trimmed_to(m_bytes.slice(m_offset));
        m_offset += nwritten;
        return nwritten;
    }

    bool write_or_error(ReadonlyBytes bytes) override
    {
        if (remaining() < bytes.size()) {
            set_recoverable_error();
            return false;
        }

        write(bytes);
        return true;
    }

    size_t fill_to_end(u8 value)
    {
        const auto nwritten = m_bytes.slice(m_offset).fill(value);
        m_offset += nwritten;
        return nwritten;
    }

    bool is_end() const { return remaining() == 0; }

    ReadonlyBytes bytes() const { return { data(), size() }; }
    Bytes bytes() { return { data(), size() }; }

    const u8* data() const { return m_bytes.data(); }
    u8* data() { return m_bytes.data(); }

    size_t size() const { return m_offset; }
    size_t remaining() const { return m_bytes.size() - m_offset; }
    void reset() { m_offset = 0; }

private:
    size_t m_offset { 0 };
    Bytes m_bytes;
};

}