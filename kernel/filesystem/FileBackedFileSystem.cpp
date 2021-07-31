/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/FileBackedFileSystem.h>

namespace Kernel {

FileBackedFileSystem::FileBackedFileSystem(FileDescription& file_description)
    : m_file_description(file_description)
{
}

FileBackedFileSystem::~FileBackedFileSystem()
{
}

}