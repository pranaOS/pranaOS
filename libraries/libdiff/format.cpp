/**
 * @file format.cpp
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "format.h"
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/vector.h>

namespace Diff 
{   
    /**
     * @param text 
     * @return String 
     */
    String generate_only_additions(const String& text)
    {
        auto lines = text.split('\n', true); 
        StringBuilder builder;
        builder.appendf("@@ -1,%u +1,%u @@\n", lines.size());
        for (const auto& line : lines) {
            builder.appendf("+%s\n", line.characters());
        }
        return builder.to_string();
    }
};