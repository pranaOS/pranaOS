/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/ByteBuffer.h>
#include <base/StringView.h>
#include <base/Types.h>

namespace Base {

class UUID {
public:
    UUID();
    UUID(Array<u8, 16> uuid_buffer);
    UUID(const StringView&);
    ~UUID() = default;


private:
    void convert_string_view_to_uuid(const StringView&);
    void fill_buffer(ByteBuffer);

    Array<u8, 16> m_uuid_buffer {};
};  

}

using Base::UUID;