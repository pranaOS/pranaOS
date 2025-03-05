/**
 * @file semanticsyntaxhighlighter.h
 * @author Krisna Pranav
 * @brief semantic syntax highlighter
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Cpp
{

    class SemanticSyntaxHighlighter final : public Syntax::Highlighter 
    {
    protected:
        /**
         * @return Vector<MatchingTokenPair> 
         */
        virtual Vector<MatchingTokenPair> matching_token_pairs_impl() const override { return m_simple_syntax_highlighter.matching_token_pairs_impl(); };

        /**
         * @param token1 
         * @param token2 
         * @return true 
         * @return false 
         */
        virtual bool token_types_equal(u64 token1, u64 token2) const override { return m_simple_syntax_highlighter.token_types_equal(token1, token2); };

    private:
        void update_spans(Vector<GUI::AutocompleteProvider::TokenInfo> const&, Gfx::Palette const&);

        Cpp::SyntaxHighlighter m_simple_syntax_highlighter;
        Vector<GUI::AutocompleteProvider::TokenInfo> m_tokens_info;
        String m_saved_tokens_text;
        Vector<Token> m_saved_tokens;
        Threading::Mutex m_lock;

    }; // class SemanticSyntaxHighlighter final : public Syntax::Highlighter 

} // namespace Cpp
