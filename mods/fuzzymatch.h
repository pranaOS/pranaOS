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
        bool matched{false};
        int score{0};
    }; // struct FuzzyMatchResult

    static constexpr int const RECURSION_LIMIT = 10;
    static constexpr int const MAX_MATCHES = 256;
    static constexpr int const SEQUENTIAL_BONUS = 15;            
    static constexpr int const SEPARATOR_BONUS = 30;             
    static constexpr int const CAMEL_BONUS = 30;                 
    static constexpr int const FIRST_LETTER_BONUS = 20;          
    static constexpr int const LEADING_LETTER_PENALTY = -5;      
    static constexpr int const MAX_LEADING_LETTER_PENALTY = -15; 
    static constexpr int const UNMATCHED_LETTER_PENALTY = -1;    

    /**
     * @param string 
     * @param index_points 
     * @param index_points_size 
     * @return int 
     */
    static int calculate_score(String const& string, u8* index_points, size_t index_points_size)
    {
        int out_score = 100;
        int penalty = LEADING_LETTER_PENALTY * index_points[0];

        if(penalty < MAX_LEADING_LETTER_PENALTY)
            penalty = MAX_LEADING_LETTER_PENALTY;

        out_score += penalty;

        int unmatched = string.length() - index_points_size;

        out_score += UNMATCHED_LETTER_PENALTY * unmatched;

        for(size_t i = 0; i < index_points_size; i++)
        {
            u8 current_idx = index_points[i];

            if(current_idx == 0)
                out_score += FIRST_LETTER_BONUS;

            if(i == 0)
                continue;

            u8 previous_idx = index_points[i - 1];

            if(current_idx - 1 == previous_idx)
                out_score += SEQUENTIAL_BONUS;

            u32 current_character = string[current_idx];
            u32 neighbor_character = string[current_idx - 1];

            if(neighbor_character != to_ascii_uppercase(neighbor_character) && current_character != to_ascii_lowercase(current_character))
                out_score += CAMEL_BONUS;

            if(neighbor_character == '_' || neighbor_character == ' ')
                out_score += SEPARATOR_BONUS;
        }

        return out_score;
    }

    /**
     * @param needle 
     * @param haystack 
     * @param needle_idx 
     * @param haystack_idx 
     * @param src_matches 
     * @param matches 
     * @param next_match 
     * @param recursion_count 
     * @return FuzzyMatchResult 
     */
    static FuzzyMatchResult fuzzy_match_recursive(String const& needle, String const& haystack, size_t needle_idx, size_t haystack_idx,
                                                u8 const* src_matches, u8* matches, int next_match, int& recursion_count)
    {
        int out_score = 0;

        ++recursion_count;

        if(recursion_count >= RECURSION_LIMIT)
            return {false, out_score};

        if(needle.length() == needle_idx || haystack.length() == haystack_idx)
            return {false, out_score};

        bool had_recursive_match = false;
        constexpr size_t recursive_match_limit = 256;
        u8 best_recursive_matches[recursive_match_limit];
        int best_recursive_score = 0;
        bool first_match = true;

        while(needle_idx < needle.length() && haystack_idx < haystack.length())
        {

            if(to_ascii_lowercase(needle[needle_idx]) == to_ascii_lowercase(haystack[haystack_idx]))
            {
                if(next_match >= MAX_MATCHES)
                    return {false, out_score};

                if(first_match && src_matches)
                {
                    memcpy(matches, src_matches, next_match);
                    first_match = false;
                }

                u8 recursive_matches[recursive_match_limit]{};
                auto result = fuzzy_match_recursive(needle, haystack, needle_idx, haystack_idx + 1, matches, recursive_matches, next_match, recursion_count);
                if(result.matched)
                {
                    if(!had_recursive_match || result.score > best_recursive_score)
                    {
                        memcpy(best_recursive_matches, recursive_matches, recursive_match_limit);
                        best_recursive_score = result.score;
                    }
                    had_recursive_match = true;
                }
                matches[next_match++] = haystack_idx;
                needle_idx++;
            }
            haystack_idx++;
        }

        bool matched = needle_idx == needle.length();
        if(!matched)
            return {false, out_score};

        out_score = calculate_score(haystack, matches, next_match);

        if(had_recursive_match && (best_recursive_score > out_score))
        {
            memcpy(matches, best_recursive_matches, MAX_MATCHES);
            out_score = best_recursive_score;
        }

        return {true, out_score};
    }

    /**
     * @param needle 
     * @param haystack 
     * @return FuzzyMatchResult 
     */
    static FuzzyMatchResult fuzzy_match(String const& needle, String const& haystack)
    {
        int recursion_count = 0;
        u8 matches[MAX_MATCHES]{};
        return fuzzy_match_recursive(needle, haystack, 0, 0, nullptr, matches, 0, recursion_count);
    }

} // namespace Mods

using Mods::fuzzy_match;
using Mods::FuzzyMatchResult;
