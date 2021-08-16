/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/Socket.h>

namespace Core {

class UDPSocket final : public Socket {
    C_OBJECT(UDPSocket)
public:
    virtual ~UDPSocket() override;

private:
    explicit UDPSocket(Object* parent = nullptr);
};

}
