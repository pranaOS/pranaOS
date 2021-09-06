/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "TerminalView.h"
#include <libui/ViewController.h>
#include <memory>
#include <sys/types.h>
#include <unistd.h>

class TerminalViewController : public UI::ViewController<TerminalView> {
public:
    TerminalViewController(TerminalView& view)
        : UI::ViewController<TerminalView>(view)
    {
    }
    virtual ~TerminalViewController() = default;

    void init_listners()
    {
        LFoundation::EventLoop::the().add(
            view().ptmx(), [this] {
                char text[256];
                int cnt = read(view().ptmx(), text, 255);
                text[cnt] = '\0';
                view().put_text(std::string(text, cnt));
            },
            nullptr);
    }

    void view_did_load() override
    {
        init_listners();
    }

private:
};