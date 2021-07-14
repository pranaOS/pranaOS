/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libfile/Archive.h>

struct ZipArchive : public Archive
{
public:
    ZipArchive(IO::Path path, bool read = true);

    JResult extract(unsigned int entry_indekx, IO::Writer &writer) override;
    JResult insert(const char *entry_name, IO::Reader &reader) override;


}