/**
 * @file syntaxhighlighter.h
 * @author Krisna Pranav
 * @brief syntax highlighter
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libsyntax/highlighter.h>

namespace Cpp 
{

    class SemanticSyntaxHighlighter;

    class SyntaxHighlighter final : public Syntax::Highlighter 
    {
        friend SemanticSyntaxHighlighter;

    public:
        /**
         * @brief Construct a new SyntaxHighlighter object
         * 
         */
        SyntaxHighlighter() = default;

        /**
         * @brief Destroy the SyntaxHighlighter object
         * 
         */
        virtual ~SyntaxHighlighter() override = default;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_identifier(u64) const override;
        virtual bool is_navigatable(u64) const override;

        /**
         * @return Syntax::Language 
         */
        virtual Syntax::Language language() const override 
        { 
            return Syntax::Language::Cpp; 
        }

        virtual void rehighlight(Palette const&) override;

    protected:
        /**
         * @return Vector<MatchingTokenPair> 
         */
        virtual Vector<MatchingTokenPair> matching_token_pairs_impl() const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool token_types_equal(u64, u64) const override;
    }; // class SyntaxHighlighter final : public Syntax::Highlighter 

} // namesapce Cpp
