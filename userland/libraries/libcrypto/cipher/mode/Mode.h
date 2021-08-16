/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/Span.h>
#include <base/StdLibExtras.h>
#include <libcrypto/cipher/Cipher.h>

namespace Crypto {
namespace Cipher {

template<typename T>
class Mode {
public:
    virtual ~Mode() { }

    virtual void encrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}, Bytes* ivec_out = nullptr) = 0;
    virtual void decrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}) = 0;

    virtual size_t IV_length() const = 0;

    const T& cipher() const { return m_cipher; }

    ByteBuffer create_aligned_buffer(size_t input_size) const
    {
        size_t remainder = (input_size + T::block_size()) % T::block_size();
        if (remainder == 0)
            return ByteBuffer::create_uninitialized(input_size);
        else
            return ByteBuffer::create_uninitialized(input_size + T::block_size() - remainder);
    }

    virtual String class_name() const = 0;
    T& cipher() { return m_cipher; }

protected:
    virtual void prune_padding(Bytes& data)
    {
        auto size = data.size();
        switch (m_cipher.padding_mode()) {
        case PaddingMode::CMS: {
            auto maybe_padding_length = data[size - 1];
            if (maybe_padding_length >= T::block_size()) {

                return;
            }
            for (auto i = size - maybe_padding_length; i < size; ++i) {
                if (data[i] != maybe_padding_length) {

                    return;
                }
            }
            data = data.slice(0, size - maybe_padding_length);
            break;
        }
        case PaddingMode::RFC5246: {
            auto maybe_padding_length = data[size - 1];

            for (auto i = size - maybe_padding_length - 1; i < size; ++i) {
                if (data[i] != maybe_padding_length) {

                    return;
                }
            }
            data = data.slice(0, size - maybe_padding_length - 1);
            break;
        }
        case PaddingMode::Null: {
            while (data[size - 1] == 0)
                --size;
            data = data.slice(0, size);
            break;
        }
        default:
            VERIFY_NOT_REACHED();
            break;
        }
    }

    template<typename... Args>
    Mode(Args... args)
        : m_cipher(args...)
    {
    }

private:
    T m_cipher;
};
}

}
