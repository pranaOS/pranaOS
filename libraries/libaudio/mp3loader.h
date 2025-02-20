/**
 * @file mp3loader.h
 * @author Krisna Pranav
 * @brief mp3 loader
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "buffer.h"
#include "loader.h"
#include "mp3types.h"
#include <mods/tuple.h>
#include <libcore/filestream.h>
#include <libdsp/mdct.h>

namespace Audio
{
    namespace MP3::Tables
    {
        struct ScaleFactorBand;
    } // namespace MP3::Tables

    class MP3LoaderPlugin : public LoaderPlugin
    {
    
    } // class MP3LoaderPlugin : public LoaderPlugin
} // namespace Audio
