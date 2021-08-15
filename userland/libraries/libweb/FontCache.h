/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/HashMap.h>
#include <base/String.h>
#include <libgfx/Font.h>
#include <libgfx/Forward.h>

struct FontSelector {
    FlyString family;
    int size { 0 };
    int weight { 0 };

    bool operator==(const FontSelector& other) const
    {
        return family == other.family && size == other.size && weight == other.weight;
    }
};

namespace Base {
template<>
struct Traits<FontSelector> : public GenericTraits<FontSelector> {
    static unsigned hash(const FontSelector& key) { return pair_int_hash(pair_int_hash(key.family.hash(), key.weight), key.size); }
};
}

class FontCache {
public:
    static FontCache& the();
    RefPtr<Gfx::Font> get(const FontSelector&) const;
    void set(const FontSelector&, NonnullRefPtr<Gfx::Font>);

private:
    FontCache() { }
    mutable HashMap<FontSelector, NonnullRefPtr<Gfx::Font>> m_fonts;
};
