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

}