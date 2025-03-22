/**
 * @file error.cpp
 * @author Krisna Pranav
 * @brief error
 * @version 6.0
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/error.h>

#ifdef KERNEL
#    include <mods/format.h>
#endif

namespace Mods 
{

    /**
     * @param string_literal 
     * @param code 
     * @return Error 
     */
    Error Error::from_string_view_or_print_error_and_return_errno(StringView string_literal, [[maybe_unused]] int code)
    {
    #ifdef KERNEL
        dmesgln("{}", string_literal);
        return Error::from_errno(code);
    #else
        return Error::from_string_view(string_literal);
    #endif
    }

}
