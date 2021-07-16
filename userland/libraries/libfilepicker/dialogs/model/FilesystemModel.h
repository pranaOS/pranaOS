/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Directory.h>
#include <libio/Path.h>
#include <libutils/Vector.h>
#include <libwidget/model/TableModel.h>
#include <libfilepicker/model/FileInfo.h>
#include <libfilepicker/model/Navigation.h>


namespace FilePicker
{

struct FilesystemModel : public Widget::TableModel
{
private:
    RefPtr<Navigation> _navigation;
    Vector<FileInfo> _files{};
    OnwPtr<Async::Observer<Navigation>> _observer;
    Func<bool(IO::Directory::Entry &)> _filter;


}

}