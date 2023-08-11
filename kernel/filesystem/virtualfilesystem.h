/**
 * @file virtualfilesystem.h
 * @author Krisna Pranav
 * @brief virtual file system
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/kresult.h>
#include <mods/badge.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/nonnullownptrvector.h>
#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <kernel/filesystem/filesystem.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/filesystem/inodemetadata.h>


namespace Kernel
{
    class Custody;
    class Device;
    class FileDescription;
    class UnveiledPath;

    struct UidAndGid
    {
        uid_t uid;
        gid_t gid;
    }; // struct Uid&Gid
    
}