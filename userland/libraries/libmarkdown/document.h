/**
 * @file document.h
 * @author Krisna Pranav
 * @brief Document
 * @version 6.0
 * @date 2024-11-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <libmarkdown/block.h>
#include <libmarkdown/containerblock.h>
#include <mods/ownptr.h>
#include <mods/string.h>

namespace Markdown
{
    class Document final
    {
    public:
        /**
         * @brief Construct a new Document object
         * 
         * @param container 
         */
        Document(OwnPtr<ContainerBlock> container)
            : m_container(move(container))
        {
        }

        /**
         * @return String 
         */
        String render_to_html() const;

        /**
         * @return String 
         */
        String render_to_inline_html() const;

        /**
         * @param view_width 
         * @return String 
         */
        String render_for_terminal(size_t view_width = 0) const;

        /**
         * @return RecursionDecision 
         */
        RecursionDecision walk(Visitor&) const;

        static OwnPtr<Document> parse(StringView);

    private:
        OwnPtr<ContainerBlock> m_container;
    }; // class Document final
} // namespace Markdown
