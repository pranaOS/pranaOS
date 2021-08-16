/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Vector.h>
#include <libgfx/Bitmap.h>
#include <libgfx/ImageDecoder.h>
#include <libgfx/Size.h>

namespace Gfx {

RefPtr<Gfx::Bitmap> load_jpg(String const& path);
RefPtr<Gfx::Bitmap> load_jpg_from_memory(const u8* data, size_t length);

struct JPGLoadingContext;

class JPGImageDecoderPlugin : public ImageDecoderPlugin {
public:
    virtual ~JPGImageDecoderPlugin() override;
    JPGImageDecoderPlugin(const u8*, size_t);
    virtual IntSize size() override;
    virtual RefPtr<Gfx::Bitmap> bitmap() override;
    virtual void set_volatile() override;
    [[nodiscard]] virtual bool set_nonvolatile(bool& was_purged) override;
    virtual bool sniff() override;
    virtual bool is_animated() override;
    virtual size_t loop_count() override;
    virtual size_t frame_count() override;
    virtual ImageFrameDescriptor frame(size_t i) override;

private:
    OwnPtr<JPGLoadingContext> m_context;
};
}
