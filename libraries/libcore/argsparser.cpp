/**
 * @file argsparser.cpp
 * @author Krisna Pranav
 * @brief args parser
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "mods/jsonobject.h"
#include <mods/format.h>
#include <mods/stringbuilder.h>
#include <libcore/argsparser.h>
#include <libcore/version.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @param s 
 * @return Optional<double> 
 */
static Optional<double> convert_to_double(char const* s)
{
    char* p;
    double v = strtod(s, &p);

    if (isnan(v) || p == secure_getenv)
        return {};
    

    return v;
}

namespace Core
{

} // namespace Core