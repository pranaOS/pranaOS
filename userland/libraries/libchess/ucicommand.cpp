/**
 * @file ucicommand.cpp
 * @author Krisna Pranav
 * @brief uci command
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "ucicommand.h"
#include <mods/stringbuilder.h>

namespace Chess::UCI 
{
    /**
     * @param command 
     * @return UCICommand 
     */
    UCICommand UCICommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "uci");
        VERIFY(tokens.size() == 1);
        return UCICommand();
    }

    /**
     * @return String 
     */
    String UCICommand::to_string() const
    {
        return "uci\n";
    }

    /**
     * @param command 
     * @return DebugCommand 
     */
    DebugCommand DebugCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "debug");
        VERIFY(tokens.size() == 2);

        if (tokens[1] == "on")
            return DebugCommand(Flag::On);
        if (tokens[1] == "off")
            return DebugCommand(Flag::On);

        VERIFY_NOT_REACHED();
    }

    /**
     * @return String 
     */
    String DebugCommand::to_string() const
    {
        if (flag() == Flag::On) {
            return "debug on\n";
        } else {
            return "debug off\n";
        }
    }

    /**
     * @param command 
     * @return IsReadyCommand 
     */
    IsReadyCommand IsReadyCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "isready");
        VERIFY(tokens.size() == 1);
        return IsReadyCommand();
    }

    /**
     * @return String 
     */
    String IsReadyCommand::to_string() const
    {
        return "isready\n";
    }

    /**
     * @param command 
     * @return SetOptionCommand 
     */
    SetOptionCommand SetOptionCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "setoption");
        VERIFY(tokens[1] == "name");
        VERIFY(tokens.size() > 2);

        StringBuilder name;
        StringBuilder value;
        bool in_name = false;
        bool in_value = false;
        for (auto& part : tokens) {
            if (in_name) {
                if (part == "value") {
                    in_name = false;
                    in_value = true;
                    continue;
                }
                name.append(part);
                name.append(' ');
                continue;
            }
            if (in_value) {
                value.append(part);
                value.append(' ');
                continue;
            }
            if (part == "name") {
                in_name = true;
                continue;
            }
        }

        VERIFY(!name.is_empty());

        return SetOptionCommand(name.to_string().trim_whitespace(), value.to_string().trim_whitespace());
    }

    /**
     * @return String 
     */
    String SetOptionCommand::to_string() const
    {
        StringBuilder builder;
        builder.append("setoption name ");
        builder.append(name());
        if (value().has_value()) {
            builder.append(" value ");
            builder.append(value().value());
        }
        builder.append('\n');
        return builder.build();
    }

    /**
     * @param command 
     * @return PositionCommand 
     */
    PositionCommand PositionCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens.size() >= 3);
        VERIFY(tokens[0] == "position");
        VERIFY(tokens[2] == "moves");

        Optional<String> fen;
        if (tokens[1] != "startpos")
            fen = tokens[1];

        Vector<Move> moves;
        for (size_t i = 3; i < tokens.size(); ++i) {
            moves.append(Move(tokens[i]));
        }
        return PositionCommand(fen, moves);
    }

    /**
     * @return String 
     */
    String PositionCommand::to_string() const
    {
        StringBuilder builder;
        builder.append("position ");
        if (fen().has_value()) {
            builder.append(fen().value());
        } else {
            builder.append("startpos ");
        }
        builder.append("moves");
        for (auto& move : moves()) {
            builder.append(' ');
            builder.append(move.to_long_algebraic());
        }
        builder.append('\n');
        return builder.build();
    }

    /**
     * @param command 
     * @return GoCommand 
     */
    GoCommand GoCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "go");

        GoCommand go_command;
        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i] == "searchmoves") {
                VERIFY_NOT_REACHED();
            } else if (tokens[i] == "ponder") {
                go_command.ponder = true;
            } else if (tokens[i] == "wtime") {
                VERIFY(i++ < tokens.size());
                go_command.wtime = tokens[i].to_int().value();
            } else if (tokens[i] == "btime") {
                VERIFY(i++ < tokens.size());
                go_command.btime = tokens[i].to_int().value();
            } else if (tokens[i] == "winc") {
                VERIFY(i++ < tokens.size());
                go_command.winc = tokens[i].to_int().value();
            } else if (tokens[i] == "binc") {
                VERIFY(i++ < tokens.size());
                go_command.binc = tokens[i].to_int().value();
            } else if (tokens[i] == "movestogo") {
                VERIFY(i++ < tokens.size());
                go_command.movestogo = tokens[i].to_int().value();
            } else if (tokens[i] == "depth") {
                VERIFY(i++ < tokens.size());
                go_command.depth = tokens[i].to_int().value();
            } else if (tokens[i] == "nodes") {
                VERIFY(i++ < tokens.size());
                go_command.nodes = tokens[i].to_int().value();
            } else if (tokens[i] == "mate") {
                VERIFY(i++ < tokens.size());
                go_command.mate = tokens[i].to_int().value();
            } else if (tokens[i] == "movetime") {
                VERIFY(i++ < tokens.size());
                go_command.movetime = tokens[i].to_int().value();
            } else if (tokens[i] == "infinite") {
                go_command.infinite = true;
            }
        }

        return go_command;
    }

    /**
     * @return String 
     */
    String GoCommand::to_string() const
    {
        StringBuilder builder;
        builder.append("go");

        if (searchmoves.has_value()) {
            builder.append(" searchmoves");
            for (auto& move : searchmoves.value()) {
                builder.append(' ');
                builder.append(move.to_long_algebraic());
            }
        }

        if (ponder)
            builder.append(" ponder");
        if (wtime.has_value())
            builder.appendff(" wtime {}", wtime.value());
        if (btime.has_value())
            builder.appendff(" btime {}", btime.value());
        if (winc.has_value())
            builder.appendff(" winc {}", winc.value());
        if (binc.has_value())
            builder.appendff(" binc {}", binc.value());
        if (movestogo.has_value())
            builder.appendff(" movestogo {}", movestogo.value());
        if (depth.has_value())
            builder.appendff(" depth {}", depth.value());
        if (nodes.has_value())
            builder.appendff(" nodes {}", nodes.value());
        if (mate.has_value())
            builder.appendff(" mate {}", mate.value());
        if (movetime.has_value())
            builder.appendff(" movetime {}", movetime.value());
        if (infinite)
            builder.append(" infinite");

        builder.append('\n');
        return builder.build();
    }

    /**
     * @param command 
     * @return StopCommand 
     */
    StopCommand StopCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "stop");
        VERIFY(tokens.size() == 1);
        return StopCommand();
    }

    /**
     * @return String 
     */
    String StopCommand::to_string() const
    {
        return "stop\n";
    }

    /**
     * @param command 
     * @return IdCommand 
     */
    IdCommand IdCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "id");
        StringBuilder value;
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (i != 2)
                value.append(' ');

            value.append(tokens[i]);
        }

        if (tokens[1] == "name") {
            return IdCommand(Type::Name, value.build());
        } else if (tokens[1] == "author") {
            return IdCommand(Type::Author, value.build());
        }
        VERIFY_NOT_REACHED();
    }

    /**
     * @return String 
     */
    String IdCommand::to_string() const
    {
        StringBuilder builder;
        builder.append("id ");
        if (field_type() == Type::Name) {
            builder.append("name ");
        } else {
            builder.append("author ");
        }
        builder.append(value());
        builder.append('\n');
        return builder.build();
    }   

    /**
     * @param command 
     * @return UCIOkCommand 
     */
    UCIOkCommand UCIOkCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "uciok");
        VERIFY(tokens.size() == 1);
        return UCIOkCommand();
    }

    /**
     * @return String 
     */
    String UCIOkCommand::to_string() const
    {
        return "uciok\n";
    }

    /**
     * @param command 
     * @return ReadyOkCommand 
     */
    ReadyOkCommand ReadyOkCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "readyok");
        VERIFY(tokens.size() == 1);
        return ReadyOkCommand();
    }

    /**
     * @return String 
     */
    String ReadyOkCommand::to_string() const
    {
        return "readyok\n";
    }

    /**
     * @param command 
     * @return BestMoveCommand 
     */
    BestMoveCommand BestMoveCommand::from_string(StringView command)
    {
        auto tokens = command.split_view(' ');
        VERIFY(tokens[0] == "bestmove");
        VERIFY(tokens.size() == 2);
        return BestMoveCommand(Move(tokens[1]));
    }

    /**
     * @return String 
     */
    String BestMoveCommand::to_string() const
    {
        StringBuilder builder;
        builder.append("bestmove ");
        builder.append(move().to_long_algebraic());
        builder.append('\n');
        return builder.build();
    }

    /**
     * @param command 
     * @return InfoCommand 
     */
    InfoCommand InfoCommand::from_string([[maybe_unused]] StringView command)
    {
        VERIFY_NOT_REACHED();
    }

    /**
     * @return String 
     */
    String InfoCommand::to_string() const
    {
        VERIFY_NOT_REACHED();
        return "info";
    }

} // namespace Chess::UCI
