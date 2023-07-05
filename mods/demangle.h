/**
 * @file demangle.h
 * @author Krisna Pranav
 * @brief demangle c++ objs
 * @version 1.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
#    include <cxxabi.h>
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

        auto* demangled_name = abi::__cxa_demangle(name.to_string().characters(), nullptr, nullptr, &status);

        auto string = String(status == 0 ? demangled_name : name);
        
        if (status == 0)
            kfree(demangled_name);

        return string;

    #endif
    }

}

using Mods::demangle;
