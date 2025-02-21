/**
 * @file wavloader.cpp
 * @author Krisna Pranav
 * @brief wavloader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "wavloader.h"
#include "buffer.h"
#include <mods/debug.h>
#include <mods/fixedarray.h>
#include <mods/numericlimits.h>
#include <mods/ownptr.h>

namespace Audio
{
    static constexpr size_t maximumm_wav_size = 1 * GiB;

    WavLoaderPlugin::WavLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            m_error = LoaderError { String::formatted("Can't Open file: {}", m_file->error_string())};
            return;
        }

        m_stream = make<Core::InputFileStream>(*m_file);
    }
} // namespace Audio
