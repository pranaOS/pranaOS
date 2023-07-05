/**
 * @file demangle.h
 * @author Krisna Pranav
 * @brief demangle c++ objs
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/**
 * @link https://linux.die.net/man/1/c++filt
 * @link https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
 * @link https://blog.oakbits.com/how-to-mangle-and-demangle-a-c-method-name.html
 */

#pragma once

#include "string.h"
#include "string_view.h"

#ifndef BUILDING_PRANAOS_TOOLCHAIN
#   include <cxxabi.h>
#endif
    
namespace Mods {
    /**
     * @param name 
     * @return String 
     */
    inline String demangle(const StringView& name) {
    #ifdef BUILDING_PRANAOS_TOOLCHAIN
        return name;
    #else
        int status = 0;
    #endif       
    }
}

using Mods::demangle;