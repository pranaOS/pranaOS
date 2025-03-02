/**
 * @file heading.h
 * @author Krisna Pranav
 * @brief Heading
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownPtr.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>
#include <libmarkdown/text.h>

namespace Markdown 
{
    class Heading final : public Block 
    {
    public:
        /**
         * @brief Construct a new Heading object
         * 
         * @param text 
         * @param level 
         */
        Heading(Text&& text, size_t level)
            : m_text(move(text))
            , m_level(level)
        {
            VERIFY(m_level > 0);
        }

        /**
         * @brief Destroy the Heading object
         * 
         */
        virtual ~Heading() override { }

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
         * @return OwnPtr<Heading> 
         */
        static OwnPtr<Heading> parse(LineIterator& lines);

    private:
        Text m_text;
        size_t m_level { 0 };
    }; // class Heading final : public Block 
} // namespace Markdown