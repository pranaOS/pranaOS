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

#include "syntaxhighlighter.h"
#include "token.h"
#include <libgui/autocompleteprovider.h>
#include <libsyntax/highlighter.h>
#include <libthreading/mutex.h>

namespace Cpp 
{

    class SemanticSyntaxHighlighter final : public Syntax::Highlighter 
    {
    public:
        /**
         * @brief Construct a new SemanticSyntaxHighlighter object
         * 
         */
        SemanticSyntaxHighlighter() = default;

        /**
         * @brief Destroy the SemanticSyntaxHighlighter object
         * 
         */
        virtual ~SemanticSyntaxHighlighter() override = default;

        /**
         * @param token 
         * @return true 
         * @return false 
         */
        virtual bool is_identifier(u64 token) const override;

        /**
         * @param token 
         * @return true 
         * @return false 
         */
        virtual bool is_navigatable(u64 token) const override;

        /**
         * @return Syntax::Language 
         */
        virtual Syntax::Language language() const override 
        { 
            return Syntax::Language::Cpp; 
        }

        virtual void rehighlight(Palette const&) override;

        void update_tokens_info(Vector<GUI::AutocompleteProvider::TokenInfo>);

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_cpp_semantic_highlighter() const override 
        { 
            return true; 
        }

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

/**
 * @tparam  
 * @return true 
 * @return false 
 */
template<>
inline bool Syntax::Highlighter::fast_is<Cpp::SemanticSyntaxHighlighter>() const { return is_cpp_semantic_highlighter(); }
