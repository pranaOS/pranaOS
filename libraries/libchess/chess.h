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
    String char_for_piece(Type type);

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

    /**
     * @param color 
     * @return Color 
     */
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
        bool operator==(Piece const& other) const { return color == other.color && type == other.type; }
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
         * @param rank 
         * @param file 
         */
        Square(int const& rank, int const& file)
            : rank(rank)
            , file(file)
        {
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Square const& other) const 
        { 
            return rank == other.rank && file == other.file; 
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        static void for_each(Callback callback)
        {
            for (int rank = 0; rank < 8; ++rank) {
                for (int file = 0; file < 8; ++file) {
                    if (callback(Square(rank, file)) == IterationDecision::Break)
                        return;
                }
            }
        }

        /**
         * @return true 
         * @return false 
         */
        bool in_bounds() const 
        { 
            return rank >= 0 && file >= 0 && rank < 8 && file < 8; 
        }

        bool is_light() const 
        { 
            return (rank % 2) != (file % 2); 
        }

        /**
         * @return String 
         */
        String to_algebraic() const;
    }; // struct Square

    class Board;

    struct Move 
    {
        Square from;
        Square to;
        Type promote_to;
        Piece piece;
        bool is_check : 1 = false;
        bool is_mate : 1 = false;
        bool is_capture : 1 = false;
        bool is_ambiguous : 1 = false;
        Square ambiguous { 50, 50 };

        /**
         * @brief Construct a new Move object
         * 
         * @param long_algebraic 
         */
        Move(StringView long_algebraic);

        /**
         * @brief Construct a new Move object
         * 
         * @param from 
         * @param to 
         * @param promote_to 
         */
        Move(Square const& from, Square const& to, Type const& promote_to = Type::None)
            : from(from)
            , to(to)
            , promote_to(promote_to)
        {
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Move const& other) const 
        { 
            return from == other.from && to == other.to && promote_to == other.promote_to; 
        }

        /**
         * @param algebraic 
         * @param turn 
         * @param board 
         * @return Move 
         */
        static Move from_algebraic(StringView algebraic, const Color turn, Board const& board);

        /**
         * @return String 
         */
        String to_long_algebraic() const;
        String to_algebraic() const;
    }; // struct Move

    class Board 
    {
    public:
        /**
         * @brief Construct a new Board object
         * 
         */
        Board();

        /**
         * @brief Get the piece object
         * 
         * @return Piece 
         */
        Piece get_piece(Square const&) const;

        /**
         * @brief Set the piece object
         * 
         * @return Piece 
         */
        Piece set_piece(Square const&, Piece const&);

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool is_legal(Move const&, Color color = Color::None) const;

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool in_check(Color color) const;

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool is_promotion_move(Move const&, Color color = Color::None) const;

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool apply_move(Move const&, Color color = Color::None);

        /**
         * @return Optional<Move> const& 
         */
        Optional<Move> const& last_move() const 
        { 
            return m_last_move; 
        }

        String to_fen() const;

        enum class Result 
        {
            CheckMate,
            StaleMate,
            WhiteResign,
            BlackResign,
            FiftyMoveRule,
            SeventyFiveMoveRule,
            ThreeFoldRepetition,
            FiveFoldRepetition,
            InsufficientMaterial,
            NotFinished,
        }; // enum class Result 

        /**
         * @param turn 
         * @return String 
         */
        static String result_to_string(Result, Color turn);
        static String result_to_points(Result, Color turn);

        /**
         * @tparam Callback 
         * @param callback 
         * @param color 
         */
        template<typename Callback>
        void generate_moves(Callback callback, Color color = Color::None) const;
        Move random_move(Color color = Color::None) const;
        Result game_result() const;
        Color game_winner() const;

        /**
         * @return int 
         */
        int game_score() const;

        /**
         * @return true 
         * @return false 
         */
        bool game_finished() const;

        void set_resigned(Color);

        /**
         * @return int 
         */
        int material_imbalance() const;

        /**
         * @return Color 
         */
        Color turn() const 
        { 
            return m_turn; 
        }

        /**
         * @return Vector<Move> const& 
         */
        Vector<Move> const& moves() const 
        { 
            return m_moves; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Board const& other) const;

    private:
        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool is_legal_no_check(Move const&, Color color) const;

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool is_legal_promotion(Move const&, Color color) const;

        /**
         * @param color 
         * @return true 
         * @return false 
         */
        bool apply_illegal_move(Move const&, Color color);

        Piece m_board[8][8];
        Optional<Move> m_last_move;

        short m_moves_since_capture { 0 };
        short m_moves_since_pawn_advance { 0 };

        Color m_turn : 2 { Color::White };
        Color m_resigned : 2 { Color::None };

        bool m_white_can_castle_kingside : 1 { true };
        bool m_white_can_castle_queenside : 1 { true };
        bool m_black_can_castle_kingside : 1 { true };
        bool m_black_can_castle_queenside : 1 { true };

        HashMap<unsigned, int> m_previous_states;
        Vector<Move> m_moves;
        friend struct Traits<Board>;
    }; // class Board

    /**
     * @tparam Callback 
     * @param callback 
     * @param color 
     */
    template<typename Callback>
    void Board::generate_moves(Callback callback, Color color) const
    {
        if (color == Color::None)
            color = turn();

        auto try_move = [&](Move m) {
            if (is_legal(m, color)) {
                if (callback(m) == IterationDecision::Break)
                    return false;
            }
            return true;
        };

        Square::for_each([&](Square sq) {
            auto piece = get_piece(sq);
            if (piece.color != color)
                return IterationDecision::Continue;

            bool keep_going = true;
            if (piece.type == Type::Pawn) {
                for (auto& piece : Vector({ Type::None, Type::Knight, Type::Bishop, Type::Rook, Type::Queen })) {
                    keep_going = try_move({ sq, { sq.rank + 1, sq.file }, piece })
                        && try_move({ sq, { sq.rank + 2, sq.file }, piece })
                        && try_move({ sq, { sq.rank - 1, sq.file }, piece })
                        && try_move({ sq, { sq.rank - 2, sq.file }, piece })
                        && try_move({ sq, { sq.rank + 1, sq.file + 1 }, piece })
                        && try_move({ sq, { sq.rank + 1, sq.file - 1 }, piece })
                        && try_move({ sq, { sq.rank - 1, sq.file + 1 }, piece })
                        && try_move({ sq, { sq.rank - 1, sq.file - 1 }, piece });
                }
            } else if (piece.type == Type::Knight) {
                keep_going = try_move({ sq, { sq.rank + 2, sq.file + 1 } })
                    && try_move({ sq, { sq.rank + 2, sq.file - 1 } })
                    && try_move({ sq, { sq.rank + 1, sq.file + 2 } })
                    && try_move({ sq, { sq.rank + 1, sq.file - 2 } })
                    && try_move({ sq, { sq.rank - 2, sq.file + 1 } })
                    && try_move({ sq, { sq.rank - 2, sq.file - 1 } })
                    && try_move({ sq, { sq.rank - 1, sq.file + 2 } })
                    && try_move({ sq, { sq.rank - 1, sq.file - 2 } });
            } else if (piece.type == Type::Bishop) {
                for (int dr = -1; dr <= 1; dr += 2) {
                    for (int df = -1; df <= 1; df += 2) {
                        for (Square to = sq; to.in_bounds(); to = { to.rank + dr, to.file + df }) {
                            if (!try_move({ sq, to }))
                                return IterationDecision::Break;
                        }
                    }
                }
            } else if (piece.type == Type::Rook) {
                for (int dr = -1; dr <= 1; dr++) {
                    for (int df = -1; df <= 1; df++) {
                        if ((dr == 0) != (df == 0)) {
                            for (Square to = sq; to.in_bounds(); to = { to.rank + dr, to.file + df }) {
                                if (!try_move({ sq, to }))
                                    return IterationDecision::Break;
                            }
                        }
                    }
                }
            } else if (piece.type == Type::Queen) {
                for (int dr = -1; dr <= 1; dr++) {
                    for (int df = -1; df <= 1; df++) {
                        if (dr != 0 || df != 0) {
                            for (Square to = sq; to.in_bounds(); to = { to.rank + dr, to.file + df }) {
                                if (!try_move({ sq, to }))
                                    return IterationDecision::Break;
                            }
                        }
                    }
                }
            } else if (piece.type == Type::King) {
                for (int dr = -1; dr <= 1; dr++) {
                    for (int df = -1; df <= 1; df++) {
                        if (!try_move({ sq, { sq.rank + dr, sq.file + df } }))
                            return IterationDecision::Break;
                    }
                }

                if (sq == Square("e1")) {
                    keep_going = try_move({ sq, Square("c1") }) && try_move({ sq, Square("g1") });
                } else if (sq == Square("e8")) {
                    keep_going = try_move({ sq, Square("c8") }) && try_move({ sq, Square("g8") });
                }
            }

            if (keep_going) {
                return IterationDecision::Continue;
            } else {
                return IterationDecision::Break;
            }
        });
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
