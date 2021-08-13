/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/IPv4Address.h>
#include <libcore/Notifier.h>
#include <libcore/Object.h>

namespace Core {

class TCPServer : public Object {
    C_OBJECT(TCPServer)
public:
    virtual ~TCPServer() override;

    bool is_listening() const { return m_listening; }
    bool listen(const IPv4Address& address, u16 port);
    void set_blocking(bool blocking);

    RefPtr<TCPSocket> accept();

    Optional<IPv4Address> local_address() const;
    Optional<u16> local_port() const;

    Function<void()> on_ready_to_accept;

private:
    explicit TCPServer(Object* parent = nullptr);

    int m_fd { -1 };
    bool m_listening { false };
    RefPtr<Notifier> m_notifier;
};

}
