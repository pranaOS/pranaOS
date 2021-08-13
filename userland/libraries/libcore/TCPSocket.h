/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Badge.h>
#include <libcore/Socket.h>

namespace Core {

class TCPSocket final : public Socket {
    C_OBJECT(TCPSocket)
public:
    virtual ~TCPSocket() override;

private:
    TCPSocket(int fd, Object* parent = nullptr);
    explicit TCPSocket(Object* parent = nullptr);
};

}
