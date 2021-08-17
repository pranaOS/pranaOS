/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libipc/Encoder.h>

template <typename T>
class Encodable {
public:
    virtual void encode(EncodedMessage& buf) const { }
};