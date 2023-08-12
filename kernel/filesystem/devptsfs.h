/**
 * @file devptsfs.h
 * @author Krisna Pranav
 * @brief dev pts fs.
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/filesystem.h>
#include <kernel/filesystem/inode.h>
#include <mods/types.h>

namespace Kernel
{
    class SlavePTY;
    class DevPtsFSInode;

    class DevPtsFS final : public FS
    {
        friend class DevPtsFSInode;


    private:
        DevPtsFS();
        RefPtr<Inode> get_inode(InodeIdentifier) const;

        RefPtr<DevPtsFSInode> m_root_inode;
    }
} // namespace Kernel
