/**
 * @file wavloader.h
 * @author Krisna Pranav
 * @brief wav loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/memorystream.h>
#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/span.h>
#include <mods/stream.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/weakptr.h>
#include <libaudio/buffer.h>
#include <libaudio/loader.h>
#include <libcore/file.h>
#include <libcore/filestream.h>

namespace Audio
{
    #define WAVE_FORMAT_PCM
    #define WAVE_FORMAT_IEEE_FLOAT
    #define WAVE_FORMAT_ALAW
    #define WAVE_FORMAT_MULAW
    #define WAVE_FORMAT_EXTENSIBLE

    class WavLoaderPlugin : public LoaderPlugin
    {

    }; // class WavLoaderPlugin : public LoaderPlugin
} // namespace Audio