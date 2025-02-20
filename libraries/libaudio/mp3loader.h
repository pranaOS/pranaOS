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
    private:
        MaybeLoaderError synchronize();
        MaybeLoaderError build_seek_table();

        ErrorOr<MP3::Header, LoaderError> read_header();
        ErrorOr<MP3::MP3Frame, LoaderError> read_next_frame();
        
        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError read_side_information(MP3::MP3Frame&);

        u32 m_sample_rate { 0 };
        u8 m_num_channels { 0 };
        size_t m_file_size { 0 };
        int m_total_samples { 0 };
        size_t m_loaded_samples { 0 };

        Mods::Optional<MP3::MP3Frame> m_current_frame;
        u32 m_current_frame_read;
        RefPtr<Core::File> m_file;
        OwnPtr<Core::InputFileStream> m_file_stream;
        OwnPtr<InputBitStream> m_bitstream;
        DuplexMemoryStream m_bit_reservoir;
        Optional<LoaderError> m_error {};
    } // class MP3LoaderPlugin : public LoaderPlugin
} // namespace Audio
