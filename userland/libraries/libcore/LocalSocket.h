/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/Socket.h>

namespace Core {

class LocalSocket final : public Socket {
    C_OBJECT(LocalSocket)
public:
    virtual ~LocalSocket() override;

    static RefPtr<LocalSocket> take_over_accepted_socket_from_system_server(String const& socket_path = String());
    pid_t peer_pid() const;

private:
    explicit LocalSocket(Object* parent = nullptr);
    LocalSocket(int fd, Object* parent = nullptr);

    friend class LocalServer;

    static void parse_sockets_from_system_server();

    static HashMap<String, int> s_overtaken_sockets;
    static bool s_overtaken_sockets_parsed;
};

}
