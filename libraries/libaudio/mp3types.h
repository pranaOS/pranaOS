/**
 * @file mp3types.h
 * @author Krisna Pranav
 * @brief mp3 types
 * @version 6.0
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/enumbits.h>
#include <mods/fixedarray.h>

namespace Audio::MP3
{
    struct Granule
    {

    }; // struct Granule

    struct Channel
    {
        Array<Granule, 2> granules;
        Array<int, 39> scale_factors;
        Array<int, 4> scale_factor_selection_info;
    }; // struct Channel

    struct MP3Frame
    {
        Header header;
        FixedArray<Channel> channels;
        off_t main_data_begin;
        u32 private_bits;
        
        /**
         * @brief Construct a new MP3Frame object
         * 
         * @param header 
         */
        MP3Frame(Header header)
            : header(header)
            , channels(FixedArray<Channel>::must_create_but_fixme_should_propagate_errors());
    }; // struct MP3Frame
} // namespace Audio::MP3