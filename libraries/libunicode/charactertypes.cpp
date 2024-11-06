/**
 * @file charactertypes.cpp
 * @author Krisna Pranav
 * @brief Character Types
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/charactertypes.h>
#include <mods/platform.h>
#include <mods/string_builder.h>
#include <mods/types.h>
#include <mods/utf16view.h>
#include <mods/utf8view.h>
#include <libunicode/charactertypes.h>
#include <libunicode/locale.h>

#if ENABLE_UNICODE_DATA
#    include <libunicode/unicodedata.h>
#endif

namespace Unicode 
{

    Optional<String> __attribute__((weak)) code_point_display_name(u32) 
    { 
        return {}; 
    }

    Optional<StringView> __attribute__((weak)) code_point_block_display_name(u32) 
    { 
        return {}; 
    }

    Optional<StringView> __attribute__((weak)) code_point_abbreviation(u32) 
    { 
        return {}; 
    }

    u32 __attribute__((weak)) canonical_combining_class(u32) 
    { 
        return {}; 
    }

    Span<SpecialCasing const* const> __attribute__((weak)) special_case_mapping(u32) 
    { 
        return {}; 
    }

    Span<BlockName const> __attribute__((weak)) block_display_names() 
    { 
        return {}; 
    }

    #if ENABLE_UNICODE_DATA

    /**
     * @param string 
     * @param index 
     * @return true 
     * @return false 
     */
    static bool is_after_uppercase_i(Utf8View const& string, size_t index)
    {
        auto preceding_view = string.substring_view(0, index);
        bool found_uppercase_i = false;

        for (auto code_point : preceding_view) {
            if (code_point == 'I') {
                found_uppercase_i = true;
                continue;
            }

            u32 combining_class = canonical_combining_class(code_point);

            if (combining_class == 0)
                found_uppercase_i = false;
            else if (combining_class == 230)
                found_uppercase_i = false;
        }

        return found_uppercase_i;
    }

    /**
     * @param string 
     * @param index 
     * @return true 
     * @return false 
     */
    static bool is_after_soft_dotted_code_point(Utf8View const& string, size_t index)
    {
        auto preceding_view = string.substring_view(0, index);
        bool found_soft_dotted_code_point = false;

        for (auto code_point : preceding_view) {
            if (code_point_has_property(code_point, Property::Soft_Dotted)) {
                found_soft_dotted_code_point = true;
                continue;
            }

            u32 combining_class = canonical_combining_class(code_point);

            if (combining_class == 0)
                found_soft_dotted_code_point = false;
            else if (combining_class == 230)
                found_soft_dotted_code_point = false;
        }

        return found_soft_dotted_code_point;
    }

    /**
     * @param string 
     * @param index 
     * @param byte_length 
     * @return true 
     * @return false 
     */
    static bool is_final_code_point(Utf8View const& string, size_t index, size_t byte_length)
    {
        auto preceding_view = string.substring_view(0, index);
        auto following_view = ((index + byte_length) < string.byte_length())
            ? string.substring_view(index + byte_length)
            : Utf8View {};

        size_t cased_letter_count = 0;

        for (auto code_point : preceding_view) {
            bool is_cased = code_point_has_property(code_point, Property::Cased);
            bool is_case_ignorable = code_point_has_property(code_point, Property::Case_Ignorable);

            if (is_cased && !is_case_ignorable)
                ++cased_letter_count;
            else if (!is_case_ignorable)
                cased_letter_count = 0;
        }

        if (cased_letter_count == 0)
            return false;

        for (auto code_point : following_view) {
            bool is_cased = code_point_has_property(code_point, Property::Cased);
            bool is_case_ignorable = code_point_has_property(code_point, Property::Case_Ignorable);

            if (is_case_ignorable)
                continue;
            if (is_cased)
                return false;

            break;
        }

        return true;
    }

    /**
     * @param string 
     * @param index 
     * @param byte_length 
     * @return true 
     * @return false 
     */
    static bool is_followed_by_combining_class_above(Utf8View const& string, size_t index, size_t byte_length)
    {
        auto following_view = ((index + byte_length) < string.byte_length())
            ? string.substring_view(index + byte_length)
            : Utf8View {};

        for (auto code_point : following_view) {
            u32 combining_class = canonical_combining_class(code_point);

            if (combining_class == 0)
                return false;
            if (combining_class == 230)
                return true;
        }

        return false;
    }

    /**
     * @param string 
     * @param index 
     * @param byte_length 
     * @return true 
     * @return false 
     */
    static bool is_followed_by_combining_dot_above(Utf8View const& string, size_t index, size_t byte_length)
    {
        auto following_view = ((index + byte_length) < string.byte_length())
            ? string.substring_view(index + byte_length)
            : Utf8View {};

        for (auto code_point : following_view) {
            if (code_point == 0x307)
                return true;

            u32 combining_class = canonical_combining_class(code_point);

            if (combining_class == 0)
                return false;
            if (combining_class == 230)
                return false;
        }

        return false;
    }

    /**
     * @param code_point 
     * @param string 
     * @param locale 
     * @param index 
     * @param byte_length 
     * @return SpecialCasing const* 
     */
    static SpecialCasing const* find_matching_special_case(u32 code_point, Utf8View const& string, Optional<StringView> locale, size_t index, size_t byte_length)
    {
        auto requested_locale = Locale::None;

        if (locale.has_value()) {
            if (auto maybe_locale = locale_from_string(*locale); maybe_locale.has_value())
                requested_locale = *maybe_locale;
        }

        auto special_casings = special_case_mapping(code_point);

        for (auto const* special_casing : special_casings) {
            if (special_casing->locale != Locale::None && special_casing->locale != requested_locale)
                continue;

            switch (special_casing->condition) {
            case Condition::None:
                return special_casing;

            case Condition::AfterI:
                if (is_after_uppercase_i(string, index))
                    return special_casing;
                break;

            case Condition::AfterSoftDotted:
                if (is_after_soft_dotted_code_point(string, index))
                    return special_casing;
                break;

            case Condition::FinalSigma:
                if (is_final_code_point(string, index, byte_length))
                    return special_casing;
                break;

            case Condition::MoreAbove:
                if (is_followed_by_combining_class_above(string, index, byte_length))
                    return special_casing;
                break;

            case Condition::NotBeforeDot:
                if (!is_followed_by_combining_dot_above(string, index, byte_length))
                    return special_casing;
                break;
            }
        }

        return nullptr;
    }

    #endif

    /**
     * @param code_point
     * 
     */
    u32 __attribute__((weak)) to_unicode_lowercase(u32 code_point)
    {
        return to_ascii_lowercase(code_point);
    }

    /**
     * @param code_point
     * 
     */
    u32 __attribute__((weak)) to_unicode_uppercase(u32 code_point)
    {
        return to_ascii_uppercase(code_point);
    }

    /**
     * @param string 
     * @param locale 
     * @return String 
     */
    String to_unicode_lowercase_full(StringView string, [[maybe_unused]] Optional<StringView> locale)
    {
    #if ENABLE_UNICODE_DATA
        Utf8View view { string };
        StringBuilder builder;

        size_t index = 0;
        size_t byte_length = 0;

        for (auto it = view.begin(); it != view.end(); ++it, index += byte_length) {
            u32 code_point = *it;
            byte_length = it.underlying_code_point_length_in_bytes();

            auto const* special_casing = find_matching_special_case(code_point, view, locale, index, byte_length);
            if (!special_casing) {
                builder.append_code_point(to_unicode_lowercase(code_point));
                continue;
            }

            for (size_t i = 0; i < special_casing->lowercase_mapping_size; ++i)
                builder.append_code_point(special_casing->lowercase_mapping[i]);
        }

        return builder.build();
    #else
        return string.to_lowercase_string();
    #endif
    }

    /**
     * @param string 
     * @param locale 
     * @return String 
     */
    String to_unicode_uppercase_full(StringView string, [[maybe_unused]] Optional<StringView> locale)
    {
    #if ENABLE_UNICODE_DATA
        Utf8View view { string };
        StringBuilder builder;

        size_t index = 0;
        size_t byte_length = 0;

        for (auto it = view.begin(); it != view.end(); ++it, index += byte_length) {
            u32 code_point = *it;
            byte_length = it.underlying_code_point_length_in_bytes();

            auto const* special_casing = find_matching_special_case(code_point, view, locale, index, byte_length);
            if (!special_casing) {
                builder.append_code_point(to_unicode_uppercase(code_point));
                continue;
            }

            for (size_t i = 0; i < special_casing->uppercase_mapping_size; ++i)
                builder.append_code_point(special_casing->uppercase_mapping[i]);
        }

        return builder.build();
    #else
        return string.to_uppercase_string();
    #endif
    }

    Optional<GeneralCategory> __attribute__((weak)) general_category_from_string(StringView) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_general_category(u32, GeneralCategory) 
    { 
        return {}; 
    }

    Optional<Property> __attribute__((weak)) property_from_string(StringView) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_property(u32, Property) 
    { 
        return {}; 
    }

    /**
     * @param property 
     * @return true 
     * @return false 
     */
    bool is_ecma262_property([[maybe_unused]] Property property)
    {
    #if ENABLE_UNICODE_DATA
        switch (property) {
        case Unicode::Property::ASCII:
        case Unicode::Property::ASCII_Hex_Digit:
        case Unicode::Property::Alphabetic:
        case Unicode::Property::Any:
        case Unicode::Property::Assigned:
        case Unicode::Property::Bidi_Control:
        case Unicode::Property::Bidi_Mirrored:
        case Unicode::Property::Case_Ignorable:
        case Unicode::Property::Cased:
        case Unicode::Property::Changes_When_Casefolded:
        case Unicode::Property::Changes_When_Casemapped:
        case Unicode::Property::Changes_When_Lowercased:
        case Unicode::Property::Changes_When_NFKC_Casefolded:
        case Unicode::Property::Changes_When_Titlecased:
        case Unicode::Property::Changes_When_Uppercased:
        case Unicode::Property::Dash:
        case Unicode::Property::Default_Ignorable_Code_Point:
        case Unicode::Property::Deprecated:
        case Unicode::Property::Diacritic:
        case Unicode::Property::Emoji:
        case Unicode::Property::Emoji_Component:
        case Unicode::Property::Emoji_Modifier:
        case Unicode::Property::Emoji_Modifier_Base:
        case Unicode::Property::Emoji_Presentation:
        case Unicode::Property::Extended_Pictographic:
        case Unicode::Property::Extender:
        case Unicode::Property::Grapheme_Base:
        case Unicode::Property::Grapheme_Extend:
        case Unicode::Property::Hex_Digit:
        case Unicode::Property::IDS_Binary_Operator:
        case Unicode::Property::IDS_Trinary_Operator:
        case Unicode::Property::ID_Continue:
        case Unicode::Property::ID_Start:
        case Unicode::Property::Ideographic:
        case Unicode::Property::Join_Control:
        case Unicode::Property::Logical_Order_Exception:
        case Unicode::Property::Lowercase:
        case Unicode::Property::Math:
        case Unicode::Property::Noncharacter_Code_Point:
        case Unicode::Property::Pattern_Syntax:
        case Unicode::Property::Pattern_White_Space:
        case Unicode::Property::Quotation_Mark:
        case Unicode::Property::Radical:
        case Unicode::Property::Regional_Indicator:
        case Unicode::Property::Sentence_Terminal:
        case Unicode::Property::Soft_Dotted:
        case Unicode::Property::Terminal_Punctuation:
        case Unicode::Property::Unified_Ideograph:
        case Unicode::Property::Uppercase:
        case Unicode::Property::Variation_Selector:
        case Unicode::Property::White_Space:
        case Unicode::Property::XID_Continue:
        case Unicode::Property::XID_Start:
            return true;
        default:
            return false;
        }
    #else
        return false;
    #endif
    }

    Optional<Script> __attribute__((weak)) script_from_string(StringView) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_script(u32, Script) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_script_extension(u32, Script) 
    { 
        return {}; 
    }

    Optional<Block> __attribute__((weak)) block_from_string(StringView) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_block(u32, Block) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_grapheme_break_property(u32, GraphemeBreakProperty) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_word_break_property(u32, WordBreakProperty) 
    { 
        return {}; 
    }

    bool __attribute__((weak)) code_point_has_sentence_break_property(u32, SentenceBreakProperty) 
    { 
        return {}; 
    }

    /**
     * @param view 
     * @return Vector<size_t> 
     */
    Vector<size_t> find_grapheme_segmentation_boundaries([[maybe_unused]] Utf16View const& view)
    {
    #if ENABLE_UNICODE_DATA
        using GBP = GraphemeBreakProperty;
        Vector<size_t> boundaries;

        if (view.length_in_code_points() == 0)
            return boundaries;

        auto has_any_gbp = [](u32 code_point, auto&&... properties) {
            return (code_point_has_grapheme_break_property(code_point, properties) || ...);
        };

        boundaries.append(0);

        if (view.length_in_code_points() > 1) {
            auto it = view.begin();
            auto code_point = *it;
            u32 next_code_point;
            auto current_ri_chain = 0;
            auto in_emoji_sequence = false;

            for (++it; it != view.end(); ++it, code_point = next_code_point) {
                next_code_point = *it;

                auto code_point_is_cr = has_any_gbp(code_point, GBP::CR);
                auto next_code_point_is_lf = has_any_gbp(next_code_point, GBP::LF);

                if (code_point_is_cr && next_code_point_is_lf)
                    continue;

                if (code_point_is_cr || next_code_point_is_lf || has_any_gbp(next_code_point, GBP::CR, GBP::Control) || has_any_gbp(code_point, GBP::LF, GBP::Control)) {
                    boundaries.append(view.code_unit_offset_of(it));
                    continue;
                }

                auto next_code_point_is_v = has_any_gbp(next_code_point, GBP::V);
                auto next_code_point_is_t = has_any_gbp(next_code_point, GBP::T);

                if (has_any_gbp(code_point, GBP::L) && (next_code_point_is_v || has_any_gbp(next_code_point, GBP::L, GBP::LV, GBP::LVT)))
                    continue;

                if ((next_code_point_is_v || next_code_point_is_t) && has_any_gbp(code_point, GBP::LV, GBP::V))
                    continue;

                if (next_code_point_is_t && has_any_gbp(code_point, GBP::LVT, GBP::T))
                    continue;

                auto code_point_is_zwj = has_any_gbp(code_point, GBP::ZWJ);
                if (!in_emoji_sequence && code_point_has_property(code_point, Property::Extended_Pictographic))
                    in_emoji_sequence = true;
                else if (in_emoji_sequence && !has_any_gbp(code_point, GBP::Extend) && !code_point_is_zwj)
                    in_emoji_sequence = false;

                if (has_any_gbp(next_code_point, GBP::Extend, GBP::ZWJ))
                    continue;

                if (has_any_gbp(next_code_point, GBP::SpacingMark))
                    continue;

                if (has_any_gbp(code_point, GBP::Prepend))
                    continue;

                if (in_emoji_sequence && code_point_is_zwj && code_point_has_property(next_code_point, Property::Extended_Pictographic))
                    continue;

                auto code_point_is_ri = has_any_gbp(code_point, GBP::Regional_Indicator);
                current_ri_chain = code_point_is_ri ? current_ri_chain + 1 : 0;

                if (code_point_is_ri && has_any_gbp(next_code_point, GBP::Regional_Indicator) && current_ri_chain % 2 == 1)
                    continue;

                boundaries.append(view.code_unit_offset_of(it));
            }
        }

        boundaries.append(view.length_in_code_units());
        return boundaries;
    #else
        return {};
    #endif
    }

    /**
     * @param view 
     * @return Vector<size_t> 
     */
    Vector<size_t> find_word_segmentation_boundaries([[maybe_unused]] Utf16View const& view)
    {
    #if ENABLE_UNICODE_DATA
        using WBP = WordBreakProperty;
        Vector<size_t> boundaries;

        if (view.length_in_code_points() == 0)
            return boundaries;

        auto has_any_wbp = [](u32 code_point, auto&&... properties) {
            return (code_point_has_word_break_property(code_point, properties) || ...);
        };

        boundaries.append(0);

        if (view.length_in_code_points() > 1) {
            auto it = view.begin();
            auto code_point = *it;
            u32 next_code_point;
            Optional<u32> previous_code_point;
            auto current_ri_chain = 0;

            for (++it; it != view.end(); ++it, previous_code_point = code_point, code_point = next_code_point) {
                next_code_point = *it;

                auto code_point_is_cr = has_any_wbp(code_point, WBP::CR);
                auto next_code_point_is_lf = has_any_wbp(next_code_point, WBP::LF);

                if (code_point_is_cr && next_code_point_is_lf)
                    continue;

                if (code_point_is_cr || next_code_point_is_lf || has_any_wbp(next_code_point, WBP::CR, WBP::Newline) || has_any_wbp(code_point, WBP::LF, WBP::Newline)) {
                    boundaries.append(view.code_unit_offset_of(it));
                    continue;
                }

                if (has_any_wbp(code_point, WBP::ZWJ) && code_point_has_property(next_code_point, Property::Extended_Pictographic))
                    continue;

                if (has_any_wbp(code_point, WBP::WSegSpace) && has_any_wbp(next_code_point, WBP::WSegSpace))
                    continue;

                if (has_any_wbp(next_code_point, WBP::Format, WBP::Extend, WBP::ZWJ))
                    continue;

                auto code_point_is_hebrew_letter = has_any_wbp(code_point, WBP::Hebrew_Letter);
                auto code_point_is_ah_letter = code_point_is_hebrew_letter || has_any_wbp(code_point, WBP::ALetter);
                auto next_code_point_is_hebrew_letter = has_any_wbp(next_code_point, WBP::Hebrew_Letter);
                auto next_code_point_is_ah_letter = next_code_point_is_hebrew_letter || has_any_wbp(next_code_point, WBP::ALetter);

                if (code_point_is_ah_letter && next_code_point_is_ah_letter)
                    continue;

                Optional<u32> next_next_code_point;

                if (it != view.end()) {
                    auto it_copy = it;
                    ++it_copy;
                    if (it_copy != view.end())
                        next_next_code_point = *it;
                }
                bool next_next_code_point_is_hebrew_letter = next_next_code_point.has_value() && has_any_wbp(*next_next_code_point, WBP::Hebrew_Letter);
                bool next_next_code_point_is_ah_letter = next_next_code_point_is_hebrew_letter || (next_next_code_point.has_value() && has_any_wbp(*next_next_code_point, WBP::ALetter));

                auto next_code_point_is_mid_num_let_q = has_any_wbp(next_code_point, WBP::MidNumLet, WBP::Single_Quote);

                if (code_point_is_ah_letter && next_next_code_point_is_ah_letter && (next_code_point_is_mid_num_let_q || has_any_wbp(next_code_point, WBP::MidLetter)))
                    continue;

                auto code_point_is_mid_num_let_q = has_any_wbp(code_point, WBP::MidNumLet, WBP::Single_Quote);
                auto previous_code_point_is_hebrew_letter = previous_code_point.has_value() && has_any_wbp(*previous_code_point, WBP::Hebrew_Letter);
                auto previous_code_point_is_ah_letter = previous_code_point_is_hebrew_letter || (previous_code_point.has_value() && has_any_wbp(*previous_code_point, WBP::ALetter));

                if (previous_code_point_is_ah_letter && next_code_point_is_ah_letter && (code_point_is_mid_num_let_q || has_any_wbp(code_point, WBP::MidLetter)))
                    continue;

                if (code_point_is_hebrew_letter && has_any_wbp(next_code_point, WBP::Single_Quote))
                    continue;

                if (code_point_is_hebrew_letter && next_next_code_point_is_hebrew_letter && has_any_wbp(next_code_point, WBP::Double_Quote))
                    continue;

                if (previous_code_point_is_hebrew_letter && next_code_point_is_hebrew_letter && has_any_wbp(code_point, WBP::Double_Quote))
                    continue;

                auto code_point_is_numeric = has_any_wbp(code_point, WBP::Numeric);
                auto next_code_point_is_numeric = has_any_wbp(next_code_point, WBP::Numeric);

                if (code_point_is_numeric && next_code_point_is_numeric)
                    continue;

                if (code_point_is_ah_letter && next_code_point_is_numeric)
                    continue;
                
                if (code_point_is_numeric && next_code_point_is_ah_letter)
                    continue;

                auto previous_code_point_is_numeric = previous_code_point.has_value() && has_any_wbp(code_point, WBP::Numeric);

                if (previous_code_point_is_numeric && next_code_point_is_numeric && (code_point_is_mid_num_let_q || has_any_wbp(code_point, WBP::MidNum)))
                    continue;

                bool next_next_code_point_is_numeric = next_next_code_point.has_value() && has_any_wbp(*next_next_code_point, WBP::Numeric);

                if (code_point_is_numeric && next_next_code_point_is_numeric && (next_code_point_is_mid_num_let_q || has_any_wbp(next_code_point, WBP::MidNum)))
                    continue;

                auto code_point_is_katakana = has_any_wbp(code_point, WBP::Katakana);
                auto next_code_point_is_katakana = has_any_wbp(next_code_point, WBP::Katakana);

                if (code_point_is_katakana && next_code_point_is_katakana)
                    continue;

                auto code_point_is_extend_num_let = has_any_wbp(code_point, WBP::ExtendNumLet);

                if ((code_point_is_ah_letter || code_point_is_numeric || code_point_is_katakana || code_point_is_extend_num_let) && has_any_wbp(next_code_point, WBP::ExtendNumLet))
                    continue;

                if (code_point_is_extend_num_let && (next_code_point_is_ah_letter || next_code_point_is_numeric || next_code_point_is_katakana))
                    continue;

                auto code_point_is_ri = has_any_wbp(code_point, WBP::Regional_Indicator);
                current_ri_chain = code_point_is_ri ? current_ri_chain + 1 : 0;

                if (code_point_is_ri && has_any_wbp(next_code_point, WBP::Regional_Indicator) && current_ri_chain % 2 == 1)
                    continue;

                boundaries.append(view.code_unit_offset_of(it));
            }
        }

        boundaries.append(view.length_in_code_units());
        return boundaries;
    #else
        return {};
    #endif
    }

    /**
     * @param view 
     * @return Vector<size_t> 
     */
    Vector<size_t> find_sentence_segmentation_boundaries([[maybe_unused]] Utf16View const& view)
    {
    #if ENABLE_UNICODE_DATA
        using SBP = SentenceBreakProperty;
        Vector<size_t> boundaries;

        if (view.length_in_code_points() == 0)
            return boundaries;

        auto has_any_sbp = [](u32 code_point, auto&&... properties) {
            return (code_point_has_sentence_break_property(code_point, properties) || ...);
        };

        boundaries.append(0);

        if (view.length_in_code_points() > 1) {
            auto it = view.begin();
            auto code_point = *it;
            u32 next_code_point;
            Optional<u32> previous_code_point;
            enum class TerminatorSequenceState 
            {
                None,
                Term,
                Close,
                Sp
            } terminator_sequence_state { TerminatorSequenceState::None };
            auto term_was_a_term = false;

            for (++it; it != view.end(); ++it, previous_code_point = code_point, code_point = next_code_point) {
                next_code_point = *it;

                auto code_point_is_cr = has_any_sbp(code_point, SBP::CR);
                auto next_code_point_is_lf = has_any_sbp(next_code_point, SBP::LF);

                if (code_point_is_cr && next_code_point_is_lf)
                    continue;

                auto code_point_is_para_sep = code_point_is_cr || has_any_sbp(code_point, SBP::LF, SBP::Sep);

                if (code_point_is_para_sep) {
                    boundaries.append(view.code_unit_offset_of(it));
                    continue;
                }

                if (has_any_sbp(next_code_point, SBP::Format, SBP::Extend))
                    continue;

                auto code_point_is_a_term = has_any_sbp(code_point, SBP::ATerm);

                 
                if (code_point_is_a_term && has_any_sbp(next_code_point, SBP::Numeric))
                    continue;
                
                if (code_point_is_a_term && previous_code_point.has_value() && has_any_sbp(*previous_code_point, SBP::Upper, SBP::Lower) && has_any_sbp(next_code_point, SBP::Upper))
                    continue;

                if (code_point_is_a_term || has_any_sbp(code_point, SBP::STerm)) {
                    terminator_sequence_state = TerminatorSequenceState::Term;
                    term_was_a_term = code_point_is_a_term;
                } else if (terminator_sequence_state >= TerminatorSequenceState::Term && terminator_sequence_state <= TerminatorSequenceState::Close && has_any_sbp(code_point, SBP::Close)) {
                    terminator_sequence_state = TerminatorSequenceState::Close;
                } else if (terminator_sequence_state >= TerminatorSequenceState::Term && has_any_sbp(code_point, SBP::Sp)) {
                    terminator_sequence_state = TerminatorSequenceState::Sp;
                } else {
                    terminator_sequence_state = TerminatorSequenceState::None;
                }

      
                if (terminator_sequence_state >= TerminatorSequenceState::Term && term_was_a_term) {
                    auto it_copy = it;
                    bool illegal_sequence = false;
                    for (auto sequence_code_point = *it_copy; it_copy != view.end(); ++it_copy) {
                        if (has_any_sbp(sequence_code_point, SBP::Close, SBP::SContinue, SBP::Numeric, SBP::Sp, SBP::Format, SBP::Extend))
                            continue;
                        illegal_sequence = has_any_sbp(sequence_code_point, SBP::Lower);
                    }
                    if (illegal_sequence)
                        continue;
                }

                 
                if (terminator_sequence_state >= TerminatorSequenceState::Term && (has_any_sbp(next_code_point, SBP::SContinue, SBP::STerm, SBP::ATerm)))
                    continue;

                auto next_code_point_is_sp = has_any_sbp(next_code_point, SBP::Sp);
                auto next_code_point_is_para_sep = has_any_sbp(next_code_point, SBP::Sep, SBP::CR, SBP::LF);

                 
                if (terminator_sequence_state >= TerminatorSequenceState::Term && terminator_sequence_state <= TerminatorSequenceState::Close && (next_code_point_is_sp || next_code_point_is_para_sep || has_any_sbp(next_code_point, SBP::Close)))
                    continue;

                
                if (terminator_sequence_state >= TerminatorSequenceState::Term && (next_code_point_is_sp || next_code_point_is_para_sep))
                    continue;

                 
                if (terminator_sequence_state >= TerminatorSequenceState::Term)
                    boundaries.append(view.code_unit_offset_of(it));

                 
            }
        }

        boundaries.append(view.length_in_code_units());
        return boundaries;
    #else
        return {};
    #endif
    }
} // namespace Unicode
