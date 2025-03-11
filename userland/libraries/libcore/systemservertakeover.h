/**
 * @file systemservertakeover.h
 * @author Krisna Pranav
 * @brief system server takeover
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/stream.h>

namespace Core 
{

    /**
     * @param socket_path 
     * @return ErrorOr<NonnullOwnPtr<Core::Stream::LocalSocket>> 
     */
    ErrorOr<NonnullOwnPtr<Core::Stream::LocalSocket>> take_over_socket_from_system_server(String const& socket_path = {});

} // namespace Core
