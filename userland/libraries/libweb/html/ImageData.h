/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libgfx/Forward.h>
#include <libjs/heap/Handle.h>
#include <libweb/bindings/Wrappable.h>

namespace Web::HTML {

class ImageData
    : public RefCounted<ImageData>
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::ImageDataWrapper;

    static RefPtr<ImageData> create_with_size(JS::GlobalObject&, int width, int height);

    ~ImageData();

    unsigned width() const;
    unsigned height() const;

    Gfx::Bitmap& bitmap() { return m_bitmap; }
    const Gfx::Bitmap& bitmap() const { return m_bitmap; }

    JS::Uint8ClampedArray* data();
    const JS::Uint8ClampedArray* data() const;

private:
    explicit ImageData(NonnullRefPtr<Gfx::Bitmap>, JS::Handle<JS::Uint8ClampedArray>);

    NonnullRefPtr<Gfx::Bitmap> m_bitmap;
    JS::Handle<JS::Uint8ClampedArray> m_data;
};

}
