/**
 * @file proxy.h
 * @author Krisna Pranav
 * @brief proxy
 * @version 6.0
 * @date 2025-03-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>




namespace IPC
{
    bool encode(Encoder&, Core::ProxyData const);

    ErrorOr<void> decode(Decoder&, Core::ProxyData&);
} // namespace IPC