/**
 * @file horizontalrule.h
 * @author Krisna Pranav
 * @brief Horizontal Rule
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownPtr.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>

namespace Markdown 
{
    class HorizontalRule final : public Block 
    {
    public:
        /**
         * @brief Construct a new Horizontal Rule object
         * 
         */
        HorizontalRule()
        {
        }

        /**
         * @brief Destroy the Horizontal Rule object
         * 
         */
        virtual ~HorizontalRule() override { }

        /**
         * @param tight 
         * @return String 
         */
        virtual String render_to_html(bool tight = false) const override;

        /**
         * @param view_width 
         * @return String 
         */
        virtual String render_for_terminal(size_t view_width = 0) const override;

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision walk(Visitor&) const override;

        /**
         * @param lines 
         * @return OwnPtr<HorizontalRule> 
         */
        static OwnPtr<HorizontalRule> parse(LineIterator& lines);
    }; // class HorizontalRule final : public Block 
} // namespace Markdown