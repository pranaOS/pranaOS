/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/StringView.h>
#include <base/Types.h>

namespace Crypto {
namespace Hash {

template<size_t BlockS, typename DigestT>
class HashFunction {
public:
    static constexpr auto BlockSize = BlockS / 8;
    static constexpr auto DigestSize = DigestT::Size;

    using DigestType = DigestT;

    constexpr static size_t block_size() { return BlockSize; };
    constexpr static size_t digest_size() { return DigestSize; };

    virtual void update(const u8*, size_t) = 0;

    void update(const Bytes& buffer) { update(buffer.data(), buffer.size()); };
    void update(const ReadonlyBytes& buffer) { update(buffer.data(), buffer.size()); };
    void update(const ByteBuffer& buffer) { update(buffer.data(), buffer.size()); };
    void update(const StringView& string) { update((const u8*)string.characters_without_null_termination(), string.length()); };

    virtual DigestType peek() = 0;
    virtual DigestType digest() = 0;

    virtual void reset() = 0;

    virtual String class_name() const = 0;

protected:
    virtual ~HashFunction() = default;
};
}
}
