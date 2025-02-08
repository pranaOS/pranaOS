/**
 * @file fuzzymatch.h
 * @author Krisna Pranav
 * @brief Fuzzy Match
 * @version 6.0
 * @date 2025-02-08
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/charactertypes.h>
#include <mods/string.h>

namespace Mods
{
    struct FuzzyMatchResult
    {
        bool matched { false };
        int score { 0 };
    }; // struct FuzzyMatchResult

    static constexpr int const RECURSION_LIMIT = 10;
    static constexpr int const MAX_MATCHES = 256;
    
    static int calculate_score(String const& string, u8* index_points)
    {
        int out_score = 100;
        int penalty = LEADING_LETTER_PENALITY * index_points[0];
    }

    static FuzzyMatchResult fuzzy_match(String const& needle, String const& haystack)
    {
        int recursion_count = 0;
        u8 matches[MAX_MATCHES] = {};
        return fuzzy_match_recursive(needle, haystack);
    }
} // namespace Mods

using Mods::fuzzy_match;
using Mods::FuzzyMatchResult;