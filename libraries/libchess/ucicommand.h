/**
 * @file ucicommand.h
 * @author Krisna Pranav
 * @brief uci command
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/string.h>
#include <libchess/chess.h>
#include <libcore/event.h>

namespace Chess::UCI 
{

    class Command : public Core::Event 
    {
    public:
        enum class Type 
        {
            UCI = 12000,
            Debug,
            IsReady,
            SetOption,
            Register,
            UCINewGame,
            Position,
            Go,
            Stop,
            PonderHit,
            Quit,
            Id,
            UCIOk,
            ReadyOk,
            BestMove,
            CopyProtection,
            Registration,
            Info,
            Option,
        }; // enum class Type 

        /**
         * @brief Construct a new Command object
         * 
         * @param type 
         */
        explicit Command(Type type)
            : Core::Event(to_underlying(type))
        {
        }

        /**
         * @return String 
         */
        virtual String to_string() const = 0;

        virtual ~Command() = default;
    }; // class Command : public Core::Event

    class UCICommand : public Command 
    {
    public:
        /**
         * @brief Construct a new UCICommand object
         * 
         */
        explicit UCICommand()
            : Command(Command::Type::UCI)
        {
        }

        /**
         * @param command 
         * @return UCICommand 
         */
        static UCICommand from_string(StringView command);

        /**
         * @return String 
         */
        virtual String to_string() const override;
    }; // class UCICommand : public Command 

    class DebugCommand : public Command 
    {
    public:
        enum class Flag 
        {
            On,
            Off
        }; // enum class Flag

        /**
         * @brief Construct a new Debug Command object
         * 
         * @param flag 
         */
        explicit DebugCommand(Flag flag)
            : Command(Command::Type::Debug)
            , m_flag(flag)
        {
        }

        /**
         * @param command 
         * @return DebugCommand 
         */
        static DebugCommand from_string(StringView command);

        /**
         * @return String 
         */
        virtual String to_string() const override;

        /**
         * @return Flag 
         */
        Flag flag() const 
        { 
            return m_flag; 
        }

    private:
        Flag m_flag;
    }; // class DebugCommand : public Command 

    class IsReadyCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Is Ready Command object
         * 
         */
        explicit IsReadyCommand()
            : Command(Command::Type::IsReady)
        {
        }
        
        /**
         * @param command 
         * @return IsReadyCommand 
         */
        static IsReadyCommand from_string(StringView command);

        /**
         * @return String 
         */
        virtual String to_string() const override;
    }; // class IsReadyCommand : public Command 

    class SetOptionCommand : public Command 
    {
    public:
        /**
         * @brief Set the Option Command object
         * 
         * @param name 
         * @param value 
         */
        explicit SetOptionCommand(StringView name, Optional<String> value = {})
            : Command(Command::Type::SetOption)
            , m_name(name)
            , m_value(value)
        {
        }

        /**
         * @param command 
         * @return SetOptionCommand 
         */
        static SetOptionCommand from_string(StringView command);

        /**
         * @return String 
         */
        virtual String to_string() const override;

        /**
         * @return String const& 
         */
        String const& name() const 
        { 
            return m_name; 
        }

        /**
         * @return Optional<String> const& 
         */
        Optional<String> const& value() const 
        { 
            return m_value; 
        }

    private:
        String m_name;
        Optional<String> m_value;
    }; // class SetOptionCommand : public Command 

    class PositionCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Position Command object
         * 
         * @param fen 
         * @param moves 
         */
        explicit PositionCommand(Optional<String> const& fen, Vector<Chess::Move> const& moves)
            : Command(Command::Type::Position)
            , m_fen(fen)
            , m_moves(moves)
        {
        }

        /**
         * @param command 
         * @return PositionCommand 
         */
        static PositionCommand from_string(StringView command);

        virtual String to_string() const override;

        /**
         * @return Optional<String> const& 
         */
        Optional<String> const& fen() const 
        { 
            return m_fen; 
        }

        /**
         * @return Vector<Chess::Move> const& 
         */
        Vector<Chess::Move> const& moves() const 
        { 
            return m_moves; 
        }

    private:
        Optional<String> m_fen;
        Vector<Chess::Move> m_moves;
    }; // class PositionCommand : public Command 

    class GoCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Go Command object
         * 
         */
        explicit GoCommand()
            : Command(Command::Type::Go)
        {
        }

        /**
         * @param command 
         * @return GoCommand 
         */
        static GoCommand from_string(StringView command);

        virtual String to_string() const override;

        Optional<Vector<Chess::Move>> searchmoves;
        bool ponder { false };
        Optional<int> wtime;
        Optional<int> btime;
        Optional<int> winc;
        Optional<int> binc;
        Optional<int> movestogo;
        Optional<int> depth;
        Optional<int> nodes;
        Optional<int> mate;
        Optional<int> movetime;
        bool infinite { false };
    }; // class GoCommand : public Command 

    class StopCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Stop Command object
         * 
         */
        explicit StopCommand()
            : Command(Command::Type::Stop)
        {
        }

        /**
         * @param command 
         * @return StopCommand 
         */
        static StopCommand from_string(StringView command);

        virtual String to_string() const override;
    }; // class StopCommand : public Command 

    class IdCommand : public Command 
    {
    public:
        enum class Type 
        {
            Name,
            Author,
        }; // enum class Type

        /**
         * @brief Construct a new Id Command object
         * 
         * @param field_type 
         * @param value 
         */
        explicit IdCommand(Type field_type, StringView value)
            : Command(Command::Type::Id)
            , m_field_type(field_type)
            , m_value(value)
        {
        }

        /**
         * @param command 
         * @return IdCommand 
         */
        static IdCommand from_string(StringView command);

        virtual String to_string() const override;

        /**
         * @return Type 
         */
        Type field_type() const 
        { 
            return m_field_type; 
        }

        /**
         * @return String const& 
         */
        String const& value() const 
        { 
            return m_value; 
        }

    private:
        Type m_field_type;
        String m_value;
    }; // class IdCommand : public Command 

    class UCIOkCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new UCIOkCommand object
         * 
         */
        explicit UCIOkCommand()
            : Command(Command::Type::UCIOk)
        {
        }

        /**
         * @param command 
         * @return UCIOkCommand 
         */
        static UCIOkCommand from_string(StringView command);

        virtual String to_string() const override;
    }; // class UCIOkCommand : public Command 

    class ReadyOkCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Ready Ok Command object
         * 
         */
        explicit ReadyOkCommand()
            : Command(Command::Type::ReadyOk)
        {
        }

        /**
         * @param command 
         * @return ReadyOkCommand 
         */
        static ReadyOkCommand from_string(StringView command);

        virtual String to_string() const override;
    }; // class ReadyOkCommand : public Command 

    class BestMoveCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Best Move Command object
         * 
         * @param move 
         */
        explicit BestMoveCommand(Chess::Move const& move)
            : Command(Command::Type::BestMove)
            , m_move(move)
        {
        }

        /**
         * @param command 
         * @return BestMoveCommand 
         */
        static BestMoveCommand from_string(StringView command);

        /**
         * @return String 
         */
        virtual String to_string() const override;

        /**
         * @return Chess::Move 
         */
        Chess::Move move() const { return m_move; }

    private:
        Chess::Move m_move;
    }; // class BestMoveCommand : public Command 

    class InfoCommand : public Command 
    {
    public:
        /**
         * @brief Construct a new Info Command object
         * 
         */
        explicit InfoCommand()
            : Command(Command::Type::BestMove)
        {
        }

        /**
         * @param command 
         * @return InfoCommand 
         */
        static InfoCommand from_string(StringView command);

        virtual String to_string() const override;

        Optional<int> depth;
        Optional<int> seldepth;
        Optional<int> time;
        Optional<int> nodes;
        Optional<Vector<Chess::Move>> pv;
        
        Optional<int> score_cp;
        Optional<int> score_mate;

        Optional<Chess::Move> currmove;
        Optional<int> currmove_number;

    }; // class InfoCommand : public Command 

} // namespace Chess::UCI
