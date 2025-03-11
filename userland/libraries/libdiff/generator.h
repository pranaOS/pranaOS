/**
 * @file generator.h
 * @author Krisna Pranav
 * @brief generator
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "hunks.h"

namespace Diff 
{

    /**
     * @param old_text 
     * @param new_text 
     * @return Vector<Hunk> 
     */
    Vector<Hunk> from_text(StringView old_text, StringView new_text);

} // namespace Diff
