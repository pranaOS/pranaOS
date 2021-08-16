/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Debug.h>
#include <base/Format.h>
#include <base/String.h>
#include <libgfx/GIFLoader.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    Gfx::GIFImageDecoderPlugin gif_decoder(data, size);
    auto bitmap = gif_decoder.bitmap();
    if (bitmap) {

        dbgln_if(GIF_DEBUG, "bitmap size: {}", bitmap->size());
        dbgln_if(GIF_DEBUG, "codec size: {}", gif_decoder.size());
        dbgln_if(GIF_DEBUG, "is_sniff: {}", gif_decoder.sniff());
        dbgln_if(GIF_DEBUG, "is_animated: {}", gif_decoder.is_animated());
        dbgln_if(GIF_DEBUG, "loop_count: {}", gif_decoder.loop_count());
        dbgln_if(GIF_DEBUG, "frame_count: {}", gif_decoder.frame_count());
        for (size_t i = 0; i < gif_decoder.frame_count(); ++i) {
            auto ifd = gif_decoder.frame(i);
            dbgln_if(GIF_DEBUG, "frame #{} size: {}", i, ifd.image->size());
            dbgln_if(GIF_DEBUG, "frame #{} duration: {}", i, ifd.duration);
        }
        dbgln_if(GIF_DEBUG, "Done.");
    }

    return 0;
}
