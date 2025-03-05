/**
 * @file syntaxhighlighter.cpp
 * @author Krisna Pranav
 * @brief syntax highlighter
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/debug.h>
#include <libcpp/lexer.h>
#include <libcpp/syntaxhighlighter.h>
#include <libgui/texteditor.h>
#include <libgfx/font/font.h>
#include <libgfx/palette.h>

namespace Cpp
{

    /**
     * @param token 
     * @return true 
     * @return false 
     */
    bool SyntaxHighlighter::is_identifier(u64 token) const
    {
        auto cpp_token = static_cast<Cpp::Token::Type>(token);
        return cpp_token == Cpp::Token::Type::Identifier;
    }

    /**
     * @param token 
     * @return true 
     * @return false 
     */
    bool SyntaxHighlighter::is_navigatable(u64 token) const
    {
        auto cpp_token = static_cast<Cpp::Token::Type>(token);
        return cpp_token == Cpp::Token::Type::IncludePath;
    }

    /**
     * @param palette 
     */
    void SyntaxHighlighter::rehighlight(Palette const& palette)
    {
        auto text = m_client->get_text();
        Cpp::Lexer lexer(text);

        Vector<GUI::TextDocumentSpan> spans;
        lexer.lex_iterable([&](auto token) {
            dbgln_if(SYNTAX_HIGHLIGHTING_DEBUG, "{} @ {}:{} - {}:{}", token.type_as_string(), token.start().line, token.start().column, token.end().line, token.end().column + 1);
            GUI::TextDocumentSpan span;
            span.range.set_start({ token.start().line, token.start().column });
            span.range.set_end({ token.end().line, token.end().column + 1 });
            auto style = style_for_token_type(palette, token.type());
            span.attributes.color = style.color;
            span.attributes.bold = style.bold;
            span.is_skippable = token.type() == Cpp::Token::Type::Whitespace;
            span.data = static_cast<u64>(token.type());
            spans.append(span);
        });
        m_client->do_set_spans(move(spans));

        m_has_brace_buddies = false;
        highlight_matching_token_pair();

        m_client->do_update();
    }

} // namespace Cpp
