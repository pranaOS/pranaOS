/**
 * @file codeblock.h
 * @author Krisna Pranav
 * @brief Code Block
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>
#include <libmarkdown/text.h>

namespace Markdown 
{
    class CodeBlock final : public Block 
    {
    public:
        /**
         * @brief Construct a new Code Block object
         * 
         * @param language 
         * @param style 
         * @param code 
         */
        CodeBlock(String const& language, String const& style, String const& code)
            : m_code(move(code))
            , m_language(language)
            , m_style(style)
        {
        }

        /**
         * @brief Destroy the Code Block object
         * 
         */
        virtual ~CodeBlock() override { }

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
         * @return OwnPtr<CodeBlock> 
         */
        static OwnPtr<CodeBlock> parse(LineIterator& lines);

    private:
        String m_code;
        String m_language;
        String m_style;
    }; // class CodeBlock final : public Block
} // namespace Markdown