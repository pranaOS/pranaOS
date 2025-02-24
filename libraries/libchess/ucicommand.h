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

        }; // enum class Type
            
        explicit Command(Type type)
            : Core::Event(to_underlying(type))
        {}

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
        {}

        /**
         * @param command 
         * @return UCICommand 
         */
        static UCICommand from_string(StringView command);

        virtual String to_string() const override;
    } // class UCICommand : public Command

} // namespace Chess::UCI    