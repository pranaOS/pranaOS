/**
 * @file filebackedfilesystem.cpp
 * @author Krisna Pranav
 * @brief file backed filesystem
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/filesystem/filebackedfilesystem.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel 
{
    /**
     * @param file_description 
     */
    FileBackedFS::FileBackedFS(FileDescription& file_description)
        : m_file_description(file_description)
    { }

    /**
     * @brief Destroy the FileBackedFS::FileBackedFS object
     * 
     */
    FileBackedFS::~FileBackedFS()
    { }

} // namespace Kernel
