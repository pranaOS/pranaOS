/**
 * @file block.h
 * @author Krisna Pranav
 * @brief Block
 * @version 6.0
 * @date 2024-10-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/recursiondecision.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libmarkdown/forward.h>

namespace Markdown 
{
    class Block 
    {
    public:
        /**
         * @brief Destroy the Block object
         * 
         */
        virtual ~Block() { }

        /**
         * @param tight 
         * @return String 
         */
        virtual String render_to_html(bool tight = false) const = 0;

        /**
         * @param view_width 
         * @return String 
         */
        virtual String render_for_terminal(size_t view_width = 0) const = 0;

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision walk(Visitor&) const = 0;
    }; // class Block
} // namespace Markdown 