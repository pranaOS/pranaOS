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

    MP3LoaderPlugin::MP3LoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            m_error = LoaderError { LoaderError::Category::IO };
            return;
        }

        off_t file_size = 0;

        if (!m_file->seek(0, Core::SeekMode::FromEndPosition, &file_size)) {
            m_error = LoaderError { LoaderError::Category::IO };
            return;
        }

        m_file_size = file_size;

        if (!m_file->seek(0, Core::SeekMode::SetPosition)) {
            m_error = LoaderError { LoaderError::Category::IO };
            return;
        }

        m_input_stream = make<Core::InputFileStream>(*m_file);

        if (!m_input_stream || m_input_stream->has_any_error()) {
            m_error = LoaderError { LoaderError::Category::Internal };
            return;
        }
    }
}