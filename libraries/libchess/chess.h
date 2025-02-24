/**
 * @file chess.h
 * @author Krisna Pranav
 * @brief chess
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashmap.h>
#include <mods/iterationdecision.h>
#include <mods/optional.h>
#include <mods/stringview.h>
#include <mods/traits.h>
#include <mods/vector.h>

namespace Chess
{
    enum class Type : u8 
    {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,
        None,
    }; // enum class Type : u8 

    /**
     * @param type 
     * @return String 
     */
    String char_for_peice(Type type);

    /**
     * @param str 
     * @return Chess::Type 
     */
    Chess::Type piece_for_char_promotion(StringView str);

    enum class Color : u8 
    {
        White,
        Black,
        None,
    }; // enum class Color : u8 
    
} // namespace Chess