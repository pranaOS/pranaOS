/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libwidget/Components.h>
#include <libwidget/Elements.h>
#include <libfilepicker/dialogs/FilePicker.h>
#include <libfilepicker/widgets/DirectoryBrowser.h>
#include <libfilepicker/widgets/FileBrowser.h>
#include <libfilepicker/widgets/ToolBar.h>

namespace FilePicker
{

Dialog::Dialog(DialogFlags flags) : _flags(flags)
{
    _navigation = make<Navigation>();
    _navigation->go_home_dont_record_histroy();
    buttons(Widget::DialogButton::OK | Widget::DialogButton::CANCEL);
}

String Dialog::get_title()
{
    return IO::format(
        "{} {}",
        _flags & DialogFlags::DIALOG_FLAGS_OPEN ? "Open" : "Save",
        _flags & DialogFlags::DIALOG_FLAGS_FOLDER ? "folder" : "document");
}

void Dialog::render(Widget::Window *window)
{
    window->size(Math::Vec2i(600, 400));

    window->root()->add(Widget::titlebar(Graphic::Icon::get("widgets"), get_title()));

    window->root()->add<ToolBar>(_navigation, nullptr, ToolBar::NO_HJ_OPEN_TERMINAL);

    if (_flags & DialogFlags::DIALOG_FLAGS_FOLDER)
    {
        _browser = window->root()->add<DirectoryBrowser>(_navigation);
    }
    else
    {
        auto file_browser = window->root()->add<FileBrowser>(_navigation);
        file_browser->on_element_selected = [this](String &path) {
            _selected_file = path;
            close(Widget::DialogResult::OK);
        };
        _browser = file_browser;
    }

    auto action_container = window->root()->add(Widget::panel());

    if (_flags & DialogFlags::DIALOG_FLAGS_SAVE)
    {
        _text_field = action_container->add<Widget::TextField>(Widget::TextModel::empty());
        _text_field->flags(Widget::Element::FILL);
        _text_field->focus();
    }

    action_container->add(Widget::spacer());

    create_buttons(action_container);
}
}