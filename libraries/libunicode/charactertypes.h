/**
 * @file charactertypes.h
 * @author Krisna Pranav
 * @brief Character Type
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/string.h>
#include <mods/types.h>
#include <libunicode/forward.h>

namespace Unicode 
{
    struct CodePointRange 
    {
        u32 first { 0 };
        u32 last { 0 };
    }; // struct CodePointRange 

    struct BlockName 
    {
        CodePointRange code_point_range {};
        StringView display_name;
    }; // struct BlockName 
    
    /**
     * @param code_point 
     * @return Optional<String> 
     */
    Optional<String> code_point_display_name(u32 code_point);

    /**
     * @param code_point 
     * @return Optional<StringView> 
     */
    Optional<StringView> code_point_block_display_name(u32 code_point);

    /**
     * @param code_point 
     * @return Optional<StringView> 
     */
    Optional<StringView> code_point_abbreviation(u32 code_point);

    Span<BlockName const> block_display_names();

    /**
     * @param code_point 
     * @return u32 
     */
    u32 canonical_combining_class(u32 code_point);

    /**
     * @param code_point 
     * @return Span<SpecialCasing const* const> 
     */
    Span<SpecialCasing const* const> special_case_mapping(u32 code_point);

    /**
     * @param code_point 
     * @return u32 
     */
    u32 to_unicode_lowercase(u32 code_point);

    /**
     * @param code_point 
     * @return u32 
     */
    u32 to_unicode_uppercase(u32 code_point);

    /**
     * @param locale 
     * @return String 
     */
    String to_unicode_lowercase_full(StringView, Optional<StringView> locale = {});

    /**
     * @param locale 
     * @return String 
     */
    String to_unicode_uppercase_full(StringView, Optional<StringView> locale = {});

    Optional<GeneralCategory> general_category_from_string(StringView);

    /**
     * @param code_point 
     * @param general_category 
     * @return true 
     * @return false 
     */
    bool code_point_has_general_category(u32 code_point, GeneralCategory general_category);
    
    Optional<Property> property_from_string(StringView);

    /**
     * @param code_point 
     * @param property 
     * @return true 
     * @return false 
     */
    bool code_point_has_property(u32 code_point, Property property);

    bool is_ecma262_property(Property);

    Optional<Script> script_from_string(StringView);

    /**
     * @param code_point 
     * @param script 
     * @return true 
     * @return false 
     */
    bool code_point_has_script(u32 code_point, Script script);

    /**
     * @param code_point 
     * @param script 
     * @return true 
     * @return false 
     */
    bool code_point_has_script_extension(u32 code_point, Script script);

    Optional<Block> block_from_string(StringView);

    /**
     * @param code_point 
     * @param block 
     * @return true 
     * @return false 
     */
    bool code_point_has_block(u32 code_point, Block block);

    /**
     * @param code_point 
     * @param property 
     * @return true 
     * @return false 
     */
    bool code_point_has_grapheme_break_property(u32 code_point, GraphemeBreakProperty property);
    
    /**
     * @param code_point 
     * @param property 
     * @return true 
     * @return false 
     */
    bool code_point_has_word_break_property(u32 code_point, WordBreakProperty property);

    /**
     * @param code_point 
     * @param property 
     * @return true 
     * @return false 
     */
    bool code_point_has_sentence_break_property(u32 code_point, SentenceBreakProperty property);

    /**
     * @return Vector<size_t> 
     */
    Vector<size_t> find_grapheme_segmentation_boundaries(Utf16View const&);
    Vector<size_t> find_word_segmentation_boundaries(Utf16View const&);
    Vector<size_t> find_sentence_segmentation_boundaries(Utf16View const&);
} // namespace Unicode