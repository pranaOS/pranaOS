/**
 * @file blockquote.h
 * @author Krisna Pranav
 * @brief Block Quote
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <libmarkdown/block.h>
#include <libmarkdown/containerblock.h>

namespace Markdown 
{
    class BlockQuote final : public Block 
    {
    public:
        /**
         * @brief Construct a new Block Quote object
         * 
         * @param contents 
         */
        BlockQuote(OwnPtr<ContainerBlock> contents)
            : m_contents(move(contents))
        {
        }

        /**
         * @brief Destroy the Block Quote object
         * 
         */
        virtual ~BlockQuote() override { }

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
         * @return OwnPtr<BlockQuote> 
         */
        static OwnPtr<BlockQuote> parse(LineIterator& lines);

    private:
        OwnPtr<ContainerBlock> m_contents;
    }; // class BlockQuote
} // namespace Markdown