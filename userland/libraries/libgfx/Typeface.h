/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Function.h>
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/Vector.h>
#include <libgfx/BitmapFont.h>
#include <libgfx/Font.h>
#include <libgfx/TrueTypeFont/Font.h>

namespace Gfx {

class Typeface : public RefCounted<Typeface> {
public:
    Typeface(const String& family, const String& variant)
        : m_family(family)
        , m_variant(variant)
    {
    }

    String family() const { return m_family; }
    String variant() const { return m_variant; }
    unsigned weight() const;

    bool is_fixed_width() const;
    bool is_fixed_size() const { return !m_bitmap_fonts.is_empty(); }
    void for_each_fixed_size_font(Function<void(const Font&)>) const;

    void add_bitmap_font(RefPtr<BitmapFont>);
    void set_ttf_font(RefPtr<TTF::Font>);

    RefPtr<Font> get_font(unsigned size);

private:
    String m_family;
    String m_variant;

    Vector<RefPtr<BitmapFont>> m_bitmap_fonts;
    RefPtr<TTF::Font> m_ttf_font;
};

}
