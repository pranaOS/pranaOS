/**
 * @file configfile.h
 * @author Krisna Pranav
 * @brief config file
 * @version 6.0
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/hashmap.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <libcore/stream.h>
#include <libgfx/color.h>

namespace Core
{

    class ConfigFile : public RefCounted<ConfigFile>
    {
    private:
        /**
         * @brief Construct a new Config File object
         * 
         * @param filename 
         * @param open_file 
         */
        ConfigFile(String const& filename, OwnPtr<Stream::BufferedFile> open_file);

        ErrorOr<void> reparse();

        String m_file;
        OwnPtr<Stream::BufferedFile> m_file;
        HashMap<String, HashMap<String, String>> m_groups;
        bool m_dirty { false };
    }; // class ConfigFile : public RefCounted<ConfigFile>

} // namespace Core
