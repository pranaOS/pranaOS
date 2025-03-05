/**
 * @file poly1305.h
 * @author Krisna Pranav
 * @brief poly1305
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>

namespace Crypto
{
    struct State {};

    class Poly1305 {
    private:
        void process_block();

        State m_state;
    };
}