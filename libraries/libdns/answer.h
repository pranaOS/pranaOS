/**
 * @file answer.h
 * @author Krisna Pranav
 * @brief answer
 * @version 6.0
 * @date 2025-03-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "name.h"
#include <mods/format.h>
#include <mods/string.h>
#include <mods/traits.h>
#include <mods/types.h>
#include <libipc/forward.h>

namespace DNS
{
    enum class RecordType : u16
    {
        A = 1,
        NS = 2,
        CNAMe = 5,
    } // enum class RecordType : u16
} // namespace DNS