/**
 * @file uciendpoint.h
 * @author Krisna Pranav
 * @brief uci end point
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libchess/ucicommand.h>
#include <libcore/iodevice.h>
#include <libcore/notifier.h>
#include <libcore/object.h>

namespace Chess::UCI
{
    class Endpoint : public Core::Object
    {
        C_OBJECT(Endpoint);

    protected:
        Endpoint() = default;

        /**
         * @brief Construct a new Endpoint object
         * 
         * @param in 
         * @param out 
         */
        Endpoint(NonnullRefPtr<Core::IODevice> in, NonnullRefPtr<Core::IODevice> out);

    private:
        void set_in_notifier();
        NonnullOwnPtr<Command> read_command();

        RefPtr<Core::IODevice> m_in;
        RefPtr<Core::IODevice> m_out;
        RefPtr<Core::IODevice> m_in_notifier;
    } // class Endpoint : public Core::Object
} // namespace Chess::UCI