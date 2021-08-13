/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/Notifier.h>
#include <libcore/Object.h>

namespace Core {

class LocalServer : public Object {
    C_OBJECT(LocalServer)
public:
    virtual ~LocalServer() override;

    bool take_over_from_system_server(String const& path = String());
    bool is_listening() const { return m_listening; }
    bool listen(const String& address);

    RefPtr<LocalSocket> accept();

    Function<void()> on_ready_to_accept;

private:
    explicit LocalServer(Object* parent = nullptr);

    void setup_notifier();

    int m_fd { -1 };
    bool m_listening { false };
    RefPtr<Notifier> m_notifier;
};

}
