/**
 * @file inodemetadata.h
 * @author Krisna Pranav
 * @brief InodeMetadata
 * @version 1.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/span.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>

namespace Kernel
{
    class Process;

    constexpr u32 encoded_device(unsigned major, unsigned minor)
    {
        return (minor & 0xff) | (major << 8);
    }

    inline bool is_directory(mode_t mode)
    {
        return (mode);
    }

    struct InodeMetadata {
        bool is_valid() const 
        {
            return inode.is_valid();
        }

        bool may_read(const Process&) const;
    };
}