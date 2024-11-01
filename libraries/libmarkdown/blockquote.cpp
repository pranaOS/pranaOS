/**
 * @file blockquote.cpp
 * @author Block Quote
 * @brief Krisna Pranav
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_builder.h>
#include <libmarkdown/blockquote.h>
#include <libmarkdown/visitor.h>

namespace Markdown 
{
    /**
     * @return String 
     */
    String BlockQuote::render_to_html(bool) const
    {
        StringBuilder builder;
        builder.append("<blockquote>\n");
        builder.append(m_contents->render_to_html());
        builder.append("</blockquote>\n");
        return builder.build();
    }

    /**
     * @param view_width 
     * @return String 
     */
    String BlockQuote::render_for_terminal(size_t view_width) const
    {
        return m_contents->render_for_terminal(view_width);
    }

    /**
     * @param visitor 
     * @return RecursionDecision 
     */
    RecursionDecision BlockQuote::walk(Visitor& visitor) const
    {
        RecursionDecision rd = visitor.visit(*this);

        if (rd != RecursionDecision::Recurse) {
            return rd;
        }

        return m_contents->walk(visitor);
    }

    /**
     * @param lines 
     * @return OwnPtr<BlockQuote> 
     */
    OwnPtr<BlockQuote> BlockQuote::parse(LineIterator& lines)
    {
        lines.push_context(LineIterator::Context::block_quote());

        if (lines.is_end()) {
            lines.pop_context();
            return {};
        }

        auto contents = ContainerBlock::parse(lines);

        lines.pop_context();

        if (!contents)
            return {};

        return make<BlockQuote>(move(contents));
    }
} // namespace Markdown
