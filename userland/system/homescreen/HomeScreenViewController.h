/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include "HomeScreenView.h"
#include <libui/App.h>
#include <libui/Button.h>
#include <libui/Label.h>
#include <libui/StackView.h>
#include <libui/View.h>
#include <libui/ViewController.h>
#include <libui/Window.h>
#include <memory>
#include <sys/types.h>

class HomeScreenViewController : public UI::ViewController<HomeScreenView> {
public:
    HomeScreenViewController(HomeScreenView& view)
        : UI::ViewController<HomeScreenView>(view)
    {
    }
    virtual ~HomeScreenViewController() = default;

    void init_data()
    {
        view().new_fast_launch_entity("About", "/res/icons/apps/about.icon", "/Applications/about.app/Content/about"); // FIXME: Parse some init file
        view().new_fast_launch_entity("Terminal", "/res/icons/apps/terminal.icon", "/Applications/terminal.app/Content/terminal");
        view().new_fast_launch_entity("Calculator", "/res/icons/apps/calculator.icon", "/Applications/calculator.app/Content/calculator");
        view().new_grid_entity("About", "/res/icons/apps/about.icon", "/Applications/about.app/Content/about"); // FIXME: Parse some init file
        view().new_grid_entity("Terminal", "/res/icons/apps/terminal.icon", "/Applications/terminal.app/Content/terminal");
        view().new_grid_entity("Monitor", "/res/icons/apps/activity_monitor.icon", "/Applications/activity_monitor.app/Content/activity_monitor");
        view().new_grid_entity("Calculator", "/res/icons/apps/calculator.icon", "/Applications/calculator.app/Content/calculator");
        view().new_grid_entity("TextEditor", "/res/icons/apps/terminal.icon", "/Applications/texteditor.app/Content/texteditor");
    }

    virtual void view_did_load() override
    {
        view().set_background_color(LG::Color(255, 255, 255, 135));
        init_data();
        view().set_needs_display();
    }

private:
};