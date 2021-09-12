/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libui/App.h>
#include <libui/Button.h>
#include <libui/Label.h>
#include <libui/View.h>
#include <libui/ViewController.h>
#include <libui/Window.h>
#include <memory>
#include <sys/types.h>
#include <sys/utsname.h>

class ViewController : public UI::ViewController<UI::View> {
public:
    ViewController(UI::View& view)
        : UI::ViewController<UI::View>(view)
    {
    }
    virtual ~ViewController() = default;

    void view_did_load() override
    {
        view().set_background_color(LG::Color::LightSystemBackground);

        utsname_t uts;
        int rc = uname(&uts);

        auto& label = view().add_subview<UI::Label>(LG::Rect(0, 0, 16, 16));
        label.set_text_color(LG::Color::Blue);
        label.set_text("Settings");
        label.set_font(LG::Font::system_bold_font());
        label.set_width(label.preferred_width());

        auto& button = view().add_subview<UI::Button>(LG::Rect(0, 0, 10, 10));
        button.set_background_color(LG::Color::LightSystemButton);
        button.set_title("Theme Switch");
        button.set_title_color(LG::Color::LightSystemText);

        view().add_constraint(UI::Constraint(label, UI::Constraint::Attribute::Left, UI::Constraint::Relation::Equal, UI::SafeArea::Left));
        view().add_constraint(UI::Constraint(label, UI::Constraint::Attribute::Top, UI::Constraint::Relation::Equal, UI::SafeArea::Top));

        view().add_constraint(UI::Constraint(button, UI::Constraint::Attribute::Left, UI::Constraint::Relation::Equal, UI::SafeArea::Left));
        view().add_constraint(UI::Constraint(button, UI::Constraint::Attribute::Top, UI::Constraint::Relation::Equal, button, UI::Constraint::Attribute::Bottom, 1, 8));


        view().set_needs_layout();
    }

private:
};