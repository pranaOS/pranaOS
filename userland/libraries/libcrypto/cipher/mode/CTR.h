/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/StringView.h>
#include <libcrypto/cipher/mode/Mode.h>

namespace Crypto {
namespace Cipher {

struct IncrementInplace {
    void operator()(Bytes& in) const
    {
        for (size_t i = in.size(); i > 0;) {
            --i;
            if (in[i] == (u8)-1) {
                in[i] = 0;
            } else {
                in[i]++;
                break;
            }
        }
    }
};

template<typename T, typename IncrementFunctionType = IncrementInplace>
class CTR : public Mode<T> {
public:
    constexpr static size_t IVSizeInBits = 128;

    virtual ~CTR() { }

    template<typename KeyType, typename... Args>
    explicit constexpr CTR(const KeyType& user_key, size_t key_bits, Intent, Args... args)
        : Mode<T>(user_key, key_bits, Intent::Encryption, args...)
    {
    }

    virtual String class_name() const override
    {
        StringBuilder builder;
        builder.append(this->cipher().class_name());
        builder.append("_CTR");
        return builder.build();
    }

    virtual size_t IV_length() const override { return IVSizeInBits / 8; }

    virtual void encrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}, Bytes* ivec_out = nullptr) override
    {

        this->encrypt_or_stream(&in, out, ivec, ivec_out);
    }

    void key_stream(Bytes& out, const Bytes& ivec = {}, Bytes* ivec_out = nullptr)
    {
        this->encrypt_or_stream(nullptr, out, ivec, ivec_out);
    }

    virtual void decrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}) override
    {
        this->encrypt(in, out, ivec);
    }

private:
    u8 m_ivec_storage[IVSizeInBits / 8];
    typename T::BlockType m_cipher_block {};

protected:
    constexpr static IncrementFunctionType increment {};

    void encrypt_or_stream(const ReadonlyBytes* in, Bytes& out, ReadonlyBytes ivec, Bytes* ivec_out = nullptr)
    {
        size_t length;
        if (in) {
            VERIFY(in->size() <= out.size());
            length = in->size();
            if (length == 0)
                return;
        } else {
            length = out.size();
        }

        auto& cipher = this->cipher();

        VERIFY(!ivec.is_empty());
        VERIFY(ivec.size() >= IV_length());

        m_cipher_block.set_padding_mode(cipher.padding_mode());

        __builtin_memcpy(m_ivec_storage, ivec.data(), IV_length());
        Bytes iv { m_ivec_storage, IV_length() };

        size_t offset { 0 };
        auto block_size = cipher.block_size();

        while (length > 0) {
            m_cipher_block.overwrite(iv.slice(0, block_size));

            cipher.encrypt_block(m_cipher_block, m_cipher_block);
            if (in) {
                m_cipher_block.apply_initialization_vector(in->slice(offset));
            }
            auto write_size = min(block_size, length);

            VERIFY(offset + write_size <= out.size());
            __builtin_memcpy(out.offset(offset), m_cipher_block.bytes().data(), write_size);

            increment(iv);
            length -= write_size;
            offset += write_size;
        }

        if (ivec_out)
            __builtin_memcpy(ivec_out->data(), iv.data(), min(ivec_out->size(), IV_length()));
    }
};

}
}
