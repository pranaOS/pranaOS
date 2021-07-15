/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include <libfilepicker/model/ArchiveListing.h>
#include <libjson/Json.h>

namespace FilePicker
{

enum Column
{
    COLUMN_NAME,
    COLUMN_TYPE,
    COLUMN_COMPRESSED_SIZE,
    COLUMN_UNCOMPRESSED_SIZE,

    __COLUMN_COUNT,
};
}