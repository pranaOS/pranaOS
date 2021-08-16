/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <libgfx/BMPLoader.h>
#include <libgfx/DDSLoader.h>
#include <libgfx/GIFLoader.h>
#include <libgfx/ICOLoader.h>
#include <libgfx/ImageDecoder.h>
#include <libgfx/JPGLoader.h>
#include <libgfx/PBMLoader.h>
#include <libgfx/PGMLoader.h>
#include <libgfx/PNGLoader.h>
#include <libgfx/PPMLoader.h>

namespace Gfx {

RefPtr<ImageDecoder> ImageDecoder::try_create(ReadonlyBytes bytes)
{
    auto* data = bytes.data();
    auto size = bytes.size();

    auto plugin = [](auto* data, auto size) -> OwnPtr<ImageDecoderPlugin> {
        OwnPtr<ImageDecoderPlugin> plugin;

        plugin = make<PNGImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<GIFImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<BMPImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<PBMImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<PGMImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<PPMImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<ICOImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<JPGImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        plugin = make<DDSImageDecoderPlugin>(data, size);
        if (plugin->sniff())
            return plugin;

        return {};
    }(data, size);

    if (!plugin)
        return {};
    return adopt_ref_if_nonnull(new (nothrow) ImageDecoder(plugin.release_nonnull()));
}

ImageDecoder::ImageDecoder(NonnullOwnPtr<ImageDecoderPlugin> plugin)
    : m_plugin(move(plugin))
{
}

ImageDecoder::~ImageDecoder()
{
}

}
