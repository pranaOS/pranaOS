/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <base/CharacterTypes.h>
#include <base/StringBuilder.h>
#include <base/StringView.h>
#include <base/Utf16View.h>
#include <base/Utf32View.h>
#include <base/Utf8View.h>

namespace Base {

static constexpr u16 high_surrogate_min = 0xd800;
static constexpr u16 high_surrogate_max = 0xdbff;
static constexpr u16 low_surrogate_min = 0xdc00;
static constexpr u16 low_surrogate_max = 0xdfff;
static constexpr u32 replacement_code_point = 0xfffd;
static constexpr u32 first_supplementary_plane_code_point = 0x10000;

template<typename UtfViewType>
static Vector<u16> to_utf16_impl(UtfViewType const& view) requires(IsSame<UtfViewType, Utf8View> || IsSame<UtfViewType, Utf32View>)
{
    Vector<u16> utf16_data;

    for (auto code_point : view) {
        if (code_point < first_supplementary_plane_code_point) {
            utf16_data.append(static_cast<u16>(code_point));
        } else {
            code_point -= first_supplementary_plane_code_point;
            utf16_data.append(static_cast<u16>(high_surrogate_min | (code_point >> 10)));
            utf16_data.append(static_cast<u16>(low_surrogate_min | (code_point & 0x3ff)));
        }
    }

    return utf16_data;
}

Vector<u16> utf8_to_utf16(StringView const& utf8_view)
{
    return to_utf16_impl(Utf8View { utf8_view });
}

Vector<u16> utf8_to_utf16(Utf8View const& utf8_view)
{
    return to_utf16_impl(utf8_view);
}

Vector<u16> utf32_to_utf16(Utf32View const& utf32_view)
{
    return to_utf16_impl(utf32_view);
}

bool Utf16View::is_high_surrogate(u16 code_unit)
{
    return (code_unit >= high_surrogate_min) && (code_unit <= high_surrogate_max);
}

bool Utf16View::is_low_surrogate(u16 code_unit)
{
    return (code_unit >= low_surrogate_min) && (code_unit <= low_surrogate_max);
}

u32 Utf16View::decode_surrogate_pair(u16 high_surrogate, u16 low_surrogate)
{
    VERIFY(is_high_surrogate(high_surrogate));
    VERIFY(is_low_surrogate(low_surrogate));

    return ((high_surrogate - high_surrogate_min) << 10) + (low_surrogate - low_surrogate_min) + first_supplementary_plane_code_point;
}

}