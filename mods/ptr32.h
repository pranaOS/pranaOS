/**
 * @file ptr32.h
 * @author Krisna Pranav
 * @brief ptr 32
 * @version 6.0
 * @date 2025-02-12
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/types.h>

namespace Mods
{
    template<typename T>
    class Ptr32
    {
    public:
        
    private:
        u32 m_ptr { 0 };
    }; // class Ptr32
} // namespace Mods

using Mods::Ptr32;