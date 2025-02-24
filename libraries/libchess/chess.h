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

    Color opposing_color(Color color);

    struct Piece
    {
        constexpr Piece()
            : color(Color::None)
            , type(Type::None)
        {
        }

        /**
         * @param c
         * @param t 
         * 
         */
        constexpr Piece(Color c, Type t)
            : color(c)
            , type(t)
        {
        }

        Color color : 4;
        Type type : 4;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Piece const& other) const
        {
            return color == other.color;
        }
    }; // struct Piece

    constexpr Piece EmptyPiece = { Color::None, Type::None };

    struct Square 
    {
        i8 rank;
        i8 file;

        /**
         * @brief Construct a new Square object
         * 
         * @param name 
         */
        Square(StringView name);

        /**
         * @brief Construct a new Square object
         * 
         * @param file 
         */
        Square(int const& rank int const& file)
            : rank(rank)
            , file(file)
        {}
    }; // struct Square 

    class Board {}

} // namespace Chess