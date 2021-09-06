/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include <libfoundation/EventReceiver.h>

namespace LFoundation {

class Object : public LFoundation::EventReceiver {
public:
    Object() = default;
    ~Object() = default;
};

} // namespace LFoundation