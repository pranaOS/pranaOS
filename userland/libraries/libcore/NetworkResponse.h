/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/ByteBuffer.h>
#include <base/RefCounted.h>

namespace Core {

class NetworkResponse : public RefCounted<NetworkResponse> {
public:
    virtual ~NetworkResponse();

    bool is_error() const { return m_error; }

protected:
    explicit NetworkResponse();

    bool m_error { false };
};

}
