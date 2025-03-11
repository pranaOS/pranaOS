/**
 * @file commentblock.h
 * @author Krisna Pranav
 * @brief Comment Block
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <mods/string.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>

namespace Markdown 
{
    class CommentBlock final : public Block 
    {
    public:
        /**
         * @brief Construct a new Comment Block object
         * 
         * @param comment 
         */
        CommentBlock(String const& comment)
            : m_comment(comment)
        {
        }
        virtual ~CommentBlock() override { }

        virtual String render_to_html(bool tight = false) const override;
        virtual String render_for_terminal(size_t view_width = 0) const override;
        virtual RecursionDecision walk(Visitor&) const override;
        static OwnPtr<CommentBlock> parse(LineIterator& lines);

    private:
        String m_comment;
    }; // class CommentBlock final : public Block
} // namespace Markdown
