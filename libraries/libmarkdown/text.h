/**
 * @file text.h
 * @author Krisna Pranav
 * @brief Text
 * @version 6.0
 * @date 2024-11-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/noncopyable.h>
#include <mods/nonnullownptrvector.h>
#include <mods/ownPtr.h>
#include <mods/recursiondecision.h>
#include <mods/string.h>
#include <libmarkdown/forward.h>

namespace Markdown 
{
    class Text final 
    {
    public:
        class Node 
        {
        public:
            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const = 0;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const = 0;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const = 0;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const = 0;

            /**
             * @brief Destroy the Node object
             * 
             */
            virtual ~Node() { }
        }; // class Node 

        class EmphasisNode : public Node 
        {
        public:
            bool strong;
            NonnullOwnPtr<Node> child;

            /**
             * @brief Construct a new Emphasis Node object
             * 
             * @param strong 
             * @param child 
             */
            EmphasisNode(bool strong, NonnullOwnPtr<Node> child)
                : strong(strong)
                , child(move(child))
            {
            }

            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class EmphasisNode : public Node 

        class CodeNode : public Node 
        {
        public:
            NonnullOwnPtr<Node> code;

            /**
             * @brief Construct a new Code Node object
             * 
             * @param code 
             */
            CodeNode(NonnullOwnPtr<Node> code)
                : code(move(code))
            {
            }

            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class CodeNode : public Node 

        class BreakNode : public Node 
        {
        public:
            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class BreakNode : public Node 

        class TextNode : public Node 
        {
        public:
            String text;
            bool collapsible;   

            /**
             * @brief Construct a new Text Node object
             * 
             * @param text 
             */
            TextNode(StringView text)
                : text(text)
                , collapsible(true)
            {
            }

            /**
             * @brief Construct a new Text Node object
             * 
             * @param text 
             * @param collapsible 
             */
            TextNode(StringView text, bool collapsible)
                : text(text)
                , collapsible(collapsible)
            {
            }

            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class TextNode : public Node 

        class LinkNode : public Node 
        {
        public:
            bool is_image;
            NonnullOwnPtr<Node> text;
            String href;

            /**
             * @brief Construct a new Link Node object
             * 
             * @param is_image 
             * @param text 
             * @param href 
             */
            LinkNode(bool is_image, NonnullOwnPtr<Node> text, String href)
                : is_image(is_image)
                , text(move(text))
                , href(move(href))
            {
            }

            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class LinkNode : public Node 

        class MultiNode : public Node 
        {
        public:
            NonnullOwnPtrVector<Node> children;

            /**
             * @param builder 
             */
            virtual void render_to_html(StringBuilder& builder) const override;

            /**
             * @param builder 
             */
            virtual void render_for_terminal(StringBuilder& builder) const override;

            /**
             * @return size_t 
             */
            virtual size_t terminal_length() const override;

            /**
             * @return RecursionDecision 
             */
            virtual RecursionDecision walk(Visitor&) const override;
        }; // class MultiNode : public Node 

        size_t terminal_length() const;

        String render_to_html() const;
        String render_for_terminal() const;
        RecursionDecision walk(Visitor&) const;

        static Text parse(StringView);

    private:
        struct Token 
        {
            String data;

            bool left_flanking;
            bool right_flanking;
            bool punct_before;
            bool punct_after;
            
            bool is_run;

            char run_char() const
            {
                VERIFY(is_run);
                return data[0];
            }

            char run_length() const
            {
                VERIFY(is_run);
                return data.length();
            }

            bool is_space() const
            {
                return data[0] == ' ';
            }

            /**
             * @param str 
             * @return true 
             * @return false 
             */
            bool operator==(StringView str) const 
            { 
                return str == data; 
            }
        };

        static Vector<Token> tokenize(StringView);

        /**
         * @param opening 
         * @return true 
         * @return false 
         */
        static bool can_open(Token const& opening);

        /**
         * @param opening 
         * @param closing 
         * @return true 
         * @return false 
         */
        static bool can_close_for(Token const& opening, Token const& closing);

        /**
         * @param tokens 
         * @param in_link 
         * @return NonnullOwnPtr<MultiNode> 
         */
        static NonnullOwnPtr<MultiNode> parse_sequence(Vector<Token>::ConstIterator& tokens, bool in_link);

        /**
         * @param tokens 
         * @return NonnullOwnPtr<Node> 
         */
        static NonnullOwnPtr<Node> parse_break(Vector<Token>::ConstIterator& tokens);

        /**
         * @param tokens 
         * @return NonnullOwnPtr<Node> 
         */
        static NonnullOwnPtr<Node> parse_newline(Vector<Token>::ConstIterator& tokens);

        /**
         * @param tokens 
         * @param in_link 
         * @return NonnullOwnPtr<Node> 
         */
        static NonnullOwnPtr<Node> parse_emph(Vector<Token>::ConstIterator& tokens, bool in_link);

        /**
         * @param tokens 
         * @return NonnullOwnPtr<Node> 
         */
        static NonnullOwnPtr<Node> parse_code(Vector<Token>::ConstIterator& tokens);

        /**
         * @param tokens 
         * @return NonnullOwnPtr<Node> 
         */
        static NonnullOwnPtr<Node> parse_link(Vector<Token>::ConstIterator& tokens);

        OwnPtr<Node> m_node;
    }; // class Text final 
} // namespace Markdown 
