/**
 * @file format.cpp
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "format.h"
#include <mods/string.h>
#include <mods/stringbuilder.h>
#include <mods/vector.h>

namespace Diff 
{

    /**
     * @param text 
     * @return String 
     */
    String generate_only_additions(String const& text)
    {
        auto lines = text.split('\n', true); 
        StringBuilder builder;
        builder.appendff("@@ -0,0 +1,{} @@\n", lines.size());
        for (auto const& line : lines) {
            builder.appendff("+{}\n", line);
        }
        return builder.to_string();
    }

}; // namespace Diff 
