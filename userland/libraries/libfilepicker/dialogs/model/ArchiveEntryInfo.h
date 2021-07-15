/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libgraphic/Icon.h>
#include <libutils/String.h>

namespace FilePicker
{

struct ArchiveEntryInfo
{
    String name;
    JFileType type;
    RefPtr<Graphic::Icon> icon;
};

}