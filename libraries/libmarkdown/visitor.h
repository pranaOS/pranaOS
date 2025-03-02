/**
 * @file visitor.h
 * @author Krisna Pranav
 * @brief Visitor
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/recursiondecision.h>
#include <libmarkdown/blockquote.h>
#include <Libmarkdown/codeblock.h>
#include <libmarkdown/commentblock.h>
#include <libmarkdown/document.h>
#include <libmarkdown/heading.h>
#include <libmarkdown/horizontalrule.h>
#include <libmarkdown/list.h>
#include <libmarkdown/paragraph.h>
#include <libmarkdown/table.h>

namespace Markdown 
{
    class Visitor 
    {
    public:
        /**
         * @brief Construct a new Visitor object
         * 
         */
        Visitor() = default;

        /**
         * @brief Destroy the Visitor object
         * 
         */
        virtual ~Visitor() = default;

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Document const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(BlockQuote const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(CodeBlock const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(CommentBlock const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(ContainerBlock const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Heading const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(HorizontalRule const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(List const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Paragraph const&) 
        { 
            return RecursionDecision::Recurse; 
        }
        
        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Table const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Table::Column const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::BreakNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::CodeNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::EmphasisNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::LinkNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::MultiNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(Text::TextNode const&) 
        { 
            return RecursionDecision::Recurse; 
        }

        /**
         * @return RecursionDecision 
         */
        virtual RecursionDecision visit(String const&) 
        { 
            return RecursionDecision::Recurse; 
        }
    }; // class Visitor
} // namespace Markdown
