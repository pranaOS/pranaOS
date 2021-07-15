/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libwidget/Components.h>
#include <libwidget/dialog/Dialog.h>
#include <libfilepicker/model/Navigation.h>
#include <libfilepicker/widgets/Browser.h>

namespace FilePicker
{

enum DialogFlags
{
    DIALOG_FLAGS_OPEN = 1,
    DIALOG_FLAGS_SAVE = 2,
    DIALOG_FLAGS_FOLDER = 4,
    DIALOG_FLAGS_MULTIPLE = 8
};

struct Dialog : public ::Widget::Dialog
{
private:
    RefPtr<Navigation> _navigation = nullptr;
    Optional<String> _selected_file = NONE;
    RefPtr<Browser> _browser;
    DialogFlags _flags;
    RefPtr<Widget::TextField> _text_field;

    String get_title();
};

}