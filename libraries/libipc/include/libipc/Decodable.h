/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cstddef>

template <typename T>
class Decodable {
public:
    virtual void decode(const char* buf, size_t& offset) { }
};