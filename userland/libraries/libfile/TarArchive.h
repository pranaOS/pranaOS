/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libfile/Archive.h>

struct TARBlock
{
    char name[100];
    char typeflag;
    char linkname[100];
    size_t size;
    char *data;
};

bool tar_read(void *tarfile, TARBlock *block, size_t index);

struct TARArchive final : public Archive
{
private:
    JResult read_archive();

};
