/**
 * @file directory.h
 * @author Krisna Pranav
 * @brief directory
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/format.h>
#include <mods/lexicalpath.h>
#include <mods/noncopyable.h>
#include <mods/optional.h>
#include <libcore/stream.h>
#include <dirent.h>
#include <sys/stat.h>

namespace Core
{
    class DirIterator;
} // namespace Core