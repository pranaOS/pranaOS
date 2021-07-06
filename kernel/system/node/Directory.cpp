/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <libabi/Result.h>
#include "system/node/Directory.h"
#include "system/node/Handle.h"


FsDirectory::FsDirecotry() : FsNode(J_FILE_TYPE_DIRECTORY)
{
}

JResult FsDirectory::open(FsHandle &handle)
{
    FileListing *listing = (FileListing *)malloc(sizeof(FileListing) + sizeof(JDirEntry) * _children.count());

    listing->count = _children.count();

    int current_index = 0;

    _children.foreach([&](auto &entry) {
        auto record = &listing->entries[current_index];
        auto node = entry.node;

        strcpy(record->name, entry.name.cstring());

        record->stat.type = node->type();
        record->stat.size = node->size();

        current_index++;

        return Iteration::CONTINUE;
    });

    handle.attached = listing;

    return SUCCESS;
}