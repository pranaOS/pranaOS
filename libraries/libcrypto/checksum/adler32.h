/**
 * @file adler32.h
 * @author Krisna Pranav
 * @brief adler32
 * @version 6.0
 * @date 2025-03-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/types.h>
#include <libcrypto/checksum/checksumfunction.h>

namespace Crypto::Checksum
{

    class Adler32 : public ChecksumFunction<u32>
    {
        
    private:
        u32 m_state_a { 1 };
        u32 m_state_b { 0 };
    }; // class Adler32 : public ChecksumFunction<u32>
    
} // namespace Crypto::Checksum
