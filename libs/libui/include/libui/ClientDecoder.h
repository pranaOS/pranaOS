/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "../../../servers/window_server/shared/Connections/WSConnection.h"
#include <libfoundation/EventLoop.h>

namespace UI {

class App;

class ClientDecoder : public BaseWindowClientDecoder {
public:
    ClientDecoder();
    ~ClientDecoder() = default;

    using BaseWindowClientDecoder::handle;
    virtual std::unique_ptr<Message> handle(const MouseMoveMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MouseActionMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MouseLeaveMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MouseWheelMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const KeyboardMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const DisplayMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const WindowCloseRequestMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const MenuBarActionMessage& msg) override;

    // Notifiers
    virtual std::unique_ptr<Message> handle(const NotifyWindowStatusChangedMessage& msg) override;
    virtual std::unique_ptr<Message> handle(const NotifyWindowIconChangedMessage& msg) override;

private:
    LFoundation::EventLoop& m_event_loop;
};

} // namespace UI