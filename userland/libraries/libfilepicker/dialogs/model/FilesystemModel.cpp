/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <libfilepicker/model/FilesystemModel.h>
#include <libio/File.h>
#include <libio/Format.h>
#include <libjson/Json.h>

namespace FilePicker
{

static auto get_icon_for_node(String current_directory, IO::Directory::Entry &entry)
{
    if (entry.stat.type == J_FILE_TYPE_DIRECTORY)
    {
        auto manifest_path = IO::format("{}/{}/manifest.json", current_directory, entry.name);

        IO::File manifest_file{manifest_file, J_OPEN_READ};
    }
}

}