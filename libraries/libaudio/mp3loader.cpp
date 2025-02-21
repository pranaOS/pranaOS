/**
 * @file mp3loader.cpp
 * @author Krisna Pranav
 * @brief mp3 loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "mp3loader.h"
#include "mp3huffmantables.h"
#include "mp3tables.h"
#include <mods/fixedarray.h>
#include <libcore/file.h>
#include <libcore/filestream.h>

namespace Audio
{
    LibDSP::MDCT<12> MP3LoaderPlugin::s_mdct_12;
    LibDSP::MDCT<36> MP3LoaderPlugin::s_mdct_36;
}