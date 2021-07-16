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
    if (entry.stat.type == HJ_FILE_TYPE_DIRECTORY)
    {
        auto manifest_path = IO::format("{}/{}/manifest.json", current_directory, entry.name);

        IO::File manifest_file{manifest_path, HJ_OPEN_READ};

        if (manifest_file.exist())
        {
            auto root = Json::parse(manifest_file);

            if (root.is(Json::OBJECT))
            {
                auto icon_name = root.get("icon");

                if (icon_name.is(Json::STRING))
                {
                    return Graphic::Icon::get(icon_name.as_string());
                }
            }
        }

        return Graphic::Icon::get("folder");
    }
    else if (entry.stat.type == HJ_FILE_TYPE_PIPE ||
             entry.stat.type == HJ_FILE_TYPE_DEVICE ||
             entry.stat.type == HJ_FILE_TYPE_SOCKET)
    {
        return Graphic::Icon::get("pipe");
    }
    else if (entry.stat.type == HJ_FILE_TYPE_TERMINAL)
    {
        return Graphic::Icon::get("console-network");
    }
    else
    {
        return Graphic::Icon::get("file");
    }
}


enum Column
{
    COLUMN_NAME,
    COLUMN_TYPE,
    COLUMN_SIZE,

    __COLUMN_COUNT,
};

FilesystemModel::FilesystemModel(RefPtr<Navigation> navigation, Func<bool(IO::Directory::Entry &)> filter)
    : _navigation(navigation), _filter(filter)
{
    _observer = navigation->observe([this](auto &) {
        update();
    });

    update();
}

int FilesystemModel::rows()
{
    return _files.count();
}

int FilesystemModel::columns()
{
    return __COLUMN_COUNT;
}


}