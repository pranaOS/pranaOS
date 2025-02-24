/**
 * @file uciendpoint.cpp
 * @author Krisna Pranav
 * @brief uci endpoint
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "uciendpoint.h"
#include <mods/bytebuffer.h>
#include <mods/debug.h>
#include <mods/string.h>
#include <libcore/eventloop.h>
#include <libcore/file.h>

namespace Chess::UCI
{
    /**
     * @brief Construct a new Endpoint::Endpoint object
     * 
     * @param in 
     * @param out 
     */
    Endpoint::Endpoint(NonnullRefPtr<Core::IODevice> in, NonnullRefPtr<Core::IODevice> out)
        : m_in(in)
        , m_out(out)
        , m_in_notifier(Core::Notifier::construct(in->fd()));
    {
    }

    /**
     * @param command 
     */
    void Endpoint::send_command(Command const& command)
    {
        dbgln_if(UCI_DEBUG, "{} Sent UCI Command: {}");
        m_out->write(command.to_string());
    }
} // namespace Chess::UCI