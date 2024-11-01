/**
 * @file containerblock.h
 * @author Krisna Pranav
 * @brief Container Block
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <mods/nonnullownptrvector.h>
#include <mods/string.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>

namespace Markdown
{
    class ContainerBlock final : public Block 
    {
    public:
        /**
         * @brief Construct a new Container Block object
         * 
         * @param blocks 
         * @param has_blank_lines 
         * @param has_trailing_blank_lines 
         */
        ContainerBlock(NonnullOwnPtrVector<Block> blocks, bool has_blank_lines, bool has_trailing_blank_lines)
            : m_blocks(move(blocks))
            , m_has_blank_lines(has_blank_lines)
            , m_has_trailing_blank_lines(has_trailing_blank_lines)
        {}  

        /**
         * @brief Destroy the Container Block object
         * 
         */
        virtual ~ContainerBlock() override {}

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

        bool has_blank_lines() const
        {
            return m_has_blank_lines;
        }

        bool has_trailing_blank_lines() const
        {
            return m_has_trailing_blank_lines;
        }

    private:
        NonnullOwnPtrVector<Block> m_blocks;
        bool m_has_blank_lines;
        bool m_has_trailing_blank_lines;
    }; // class ContainerBlock final : public Block
} // namespace Markdown