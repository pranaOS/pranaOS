/**
 * @file table.h
 * @author Krisna Pranav
 * @brief Table
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullownptrvector.h>
#include <mods/ownptr.h>
#include <libmarkdown/block.h>
#include <libmarkdown/lineiterator.h>
#include <libmarkdown/text.h>

namespace Markdown 
{
    class Table final : public Block 
    {
    public:
        enum class Alignment 
        {
            Center,
            Left,
            Right,
        }; // enum class Alignment 

        struct Column 
        {
            Text header;
            Vector<Text> rows;
            Alignment alignment { Alignment::Left };
            size_t relative_width { 0 };

            RecursionDecision walk(Visitor&) const;
        }; // struct Column 

        /**
         * @brief Construct a new Table object
         * 
         */
        Table() { }

        /**
         * @brief Destroy the Table object
         * 
         */
        virtual ~Table() override { }

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
         * @return OwnPtr<Table> 
         */
        static OwnPtr<Table> parse(LineIterator& lines);

    private:
        Vector<Column> m_columns;
        size_t m_total_width { 1 };
        size_t m_row_count { 0 };
    }; // class Table final : public Block 
} // namespace Markdown
