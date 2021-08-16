/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Span.h>

namespace Crypto::Checksum {

template<typename ChecksumType>
class ChecksumFunction {
public:
    virtual void update(ReadonlyBytes data) = 0;
    virtual ChecksumType digest() = 0;

protected:
    virtual ~ChecksumFunction() = default;
};

}
