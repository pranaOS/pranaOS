/**
 * @file paragraph.h
 * @author Krisna Pranav
 * @brief Paragrah
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullownptrvector.h>
#include <mods/ownptr.h>
#include <libmarkdown/block.h>
#include <libmarkdown/text.h>

namespace Markdown 
{
    class Paragraph final : public Block 
    {
    public:
        /**
         * @brief Construct a new Paragraph object
         * 
         * @param text 
         */
        Paragraph(Text text)
            : m_text(move(text))
        {
        }

        /**
         * @brief Destroy the Paragraph object
         * 
         */
        virtual ~Paragraph() override { }

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

    private:
        Text m_text;
    }; // class Paragraph final : public Block 
} // namespace Markdown