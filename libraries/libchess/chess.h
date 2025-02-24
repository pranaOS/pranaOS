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

    class Board 
    {
    private:
        bool is_legal_no_check(Move const&, Color color) const;
        bool is_legal_promotion(Move const&, Color color) const;
        bool apply_illegal_move(Move const&, Color color);

        Piece m_board[8][8];
        Optional<Move> m_last_move;
        
        short m_moves_since_capture { 0 };
        short m_moves_since_pawn_advance { 0 };

        Color m_turn : 2 { Color::White };
        Color m_resigned : 2 { Color::None };

        Vector<Move> m_moves;
        HashMap<unsigned, int> m_previos_states;
        friend struct Traits<Board>;
    }

} // namespace Chess

template<>
struct Mods::Traits<Chess::Piece> : public GenericTraits<Chess::Piece> {
    /**
     * @param piece 
     * @return unsigned 
     */
    static unsigned hash(Chess::Piece const& piece)
    {
        return pair_int_hash(static_cast<u32>(piece.color), static_cast<u32>(piece.type));
    }
};

template<>
struct Mods::Traits<Chess::Board> : public GenericTraits<Chess::Board> {
    /**
     * @param chess 
     * @return unsigned 
     */
    static unsigned hash(Chess::Board const& chess)
    {
        unsigned hash = 0;
        hash = pair_int_hash(hash, static_cast<u32>(chess.m_white_can_castle_queenside));
        hash = pair_int_hash(hash, static_cast<u32>(chess.m_white_can_castle_kingside));
        hash = pair_int_hash(hash, static_cast<u32>(chess.m_black_can_castle_queenside));
        hash = pair_int_hash(hash, static_cast<u32>(chess.m_black_can_castle_kingside));

        Chess::Square::for_each([&](Chess::Square sq) {
            hash = pair_int_hash(hash, Traits<Chess::Piece>::hash(chess.get_piece(sq)));
            return IterationDecision::Continue;
        });

        return hash;
    }
};