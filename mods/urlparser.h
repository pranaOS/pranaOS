/**
 * @file urlparser.h
 * @author Krisna Pranav
 * @brief url_parser
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"

namespace Mods {
    /**
     * @brief urlencode
     * 
     * @return String 
     */
    String urlencode(const StringView&);

    /**
     * @brief urldecode 
     * 
     * @return String 
     */
    String urldecode(const StringView&);

}

using Mods::urldecode;
using Mods::urlencode;
