/**
 * @file loadedlibrary.h
 * @author Krisna Pranav
 * @brief Loaded Library
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "debuginfo.h"
#include <mods/types.h>
#include <libcore/mappedfile.h>
#include <libelf/image.h>

namespace Debug 
{

    struct LoadedLibrary {
        String name;
        NonnullRefPtr<Core::MappedFile> file;
        NonnullOwnPtr<ELF::Image> image;
        NonnullOwnPtr<DebugInfo> debug_info;
        FlatPtr base_address {};

        /**
         * @brief Construct a new LoadedLibrary object
         * 
         * @param name 
         * @param file 
         * @param image 
         * @param debug_info 
         * @param base_address 
         */
        LoadedLibrary(String const& name, NonnullRefPtr<Core::MappedFile> file, NonnullOwnPtr<ELF::Image> image, NonnullOwnPtr<DebugInfo>&& debug_info, FlatPtr base_address)
            : name(name)
            , file(move(file))
            , image(move(image))
            , debug_info(move(debug_info))
            , base_address(base_address)
        {
        }

    }; // struct LoadedLibrary

} // namespace Debug
