/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libfilepicker/model/ArchiveListing.h>
#include <libfilepicker/model/Navigation.h>
#include <libwidget/Views.h>
#include <libfile/Archive.h>

namespace FilePicker
{

struct ArchiveBrowser : public Widget::Table
{
private:
    RefPtr<Navigation> _navigation;
    RefPtr<Archive> _archive;
    RefPtr<ArchiveListing> _listing;
    OwnPtr<Async::Observer<Navigation>> _navigation;

}

}