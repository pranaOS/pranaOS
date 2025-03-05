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

namespace Crypto::Authentication 
{

    struct State {
        u32 r[4] {};
        u32 s[4] {};
        u64 a[8] {};
        u8 blocks[17] {};
        u8 block_count {};
    }; // struct State

    class Poly1305 
    {
    public:
        /**
         * @brief Construct a new Poly1305 object
         * 
         * @param key 
         */
        explicit Poly1305(ReadonlyBytes key);
        
        /**
         * @param message 
         */
        void update(ReadonlyBytes message);

        /**
         * @return ErrorOr<ByteBuffer> 
         */
        ErrorOr<ByteBuffer> digest();

    private:
        void process_block();

        State m_state;
    }; // class Poly1305 

} // namespace Crypto::Authentication
