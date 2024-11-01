/**
 * @file list.h
 * @author Krisna Pranav
 * @brief List
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/ownptr.h>
#include <libmarkdown/block.h>
#include <libmarkdown/containerblock.h>
#include <libmarkdown/lineiterator.h>

namespace Markdown 
{
    class List final : public Block 
    {
    public:
        /**
         * @brief Construct a new List object
         * 
         * @param items 
         * @param is_ordered 
         * @param is_tight 
         * @param start_number 
         */
        List(Vector<OwnPtr<ContainerBlock>> items, bool is_ordered, bool is_tight, size_t start_number)
            : m_items(move(items))
            , m_is_ordered(is_ordered)
            , m_is_tight(is_tight)
            , m_start_number(start_number)
        {
        }  

        /**
         * @brief Destroy the List object
         * 
         */
        virtual ~List() override { }

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
         * @return OwnPtr<List> 
         */
        static OwnPtr<List> parse(LineIterator& lines);

    private:
        Vector<OwnPtr<ContainerBlock>> m_items;
        bool m_is_ordered { false };
        bool m_is_tight { false };
        size_t m_start_number { 1 };
    }; // class List final : public Block 
} // namespace Markdown
