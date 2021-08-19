/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Array.h>
#include <EP/ByteBuffer.h>
#include <EP/StringView.h>
#include <EP/Types.h>

namespace EP {

class UUID {
public:
    UUID();
    UUID(Array<u8, 16> uuid_buffer);
    UUID(const StringView&);
    ~UUID() = default;

    bool operator==(const UUID&) const;
    bool operator!=(const UUID& other) const { return !(*this == other); }
    bool operator<=(const UUID&) const = delete;
    bool operator>=(const UUID&) const = delete;
    bool operator<(const UUID&) const = delete;
    bool operator>(const UUID&) const = delete;

    String to_string() const;
    bool is_zero() const;

private:
    void convert_string_view_to_uuid(const StringView&);
    void fill_buffer(ByteBuffer);

    Array<u8, 16> m_uuid_buffer {};
};

}

using EP::UUID;
