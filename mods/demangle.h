/**
 * @file demangle.h
 * @author Krisna Pranav
 * @brief demangle c++ objs
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifndef KERNEL

#    include <mods/bytestring.h>
#    include <mods/stringview.h>
#    include <cxxabi.h>

namespace Mods 
{

    /**
     * @param name 
     * @return ByteString 
     */
    inline ByteString demangle(StringView name)
    {
        int status = 0;
        auto* demangled_name = abi::__cxa_demangle(name.to_byte_string().characters(), nullptr, nullptr, &status);
        auto string = ByteString(status == 0 ? StringView { demangled_name, strlen(demangled_name) } : name);
        if (status == 0)
            free(demangled_name);
        return string;
    }

} // namespace Mods

#    if USING_MODS_GLOBALLY
using Mods::demangle;
#    endif

#endif
