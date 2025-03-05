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

namespace Cpp
{

    class SemanticSyntaxHighlighter;

    class SyntaxHighlighter final : public Syntax::Highlighter
    {
    
    protected:
        virtual Vector<MatchingTokenPair> matching_token_pairs_impl() const;

        virtual bool token_types_equal(u64, u64) const override;
    }; // class SyntaxHighlighter final : public Syntax::Highlighter

} // namespace Cpp
