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

}