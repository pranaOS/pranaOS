/**
 * @file markupgenerator.h
 * @author Krisna Pranav
 * @brief Markup Generator
 * @version 6.0
 * @date 2024-10-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashtable.h>
#include <mods/string.h>
#include <libjs/forward.h>

namespace JS 
{
    class MarkupGenerator 
    {
    public:
        /**
         * @return String 
         */
        static String html_from_source(StringView);
        static String html_from_value(Value);
        static String html_from_error(Object&);

    private:
        enum class StyleType 
        {
            Invalid,
            String,
            Number,
            KeywordBold,
            Punctuation,
            Operator,
            Keyword,
            ControlKeyword,
            Identifier,
            ObjectType,
        }; // enum class StyleType 

        /**
         * @param output_html 
         * @param seen_objects 
         */
        static void value_to_html(Value, StringBuilder& output_html, HashTable<Object*> seen_objects = {});

        /**
         * @param output_html 
         */
        static void array_to_html(const Array&, StringBuilder& output_html, HashTable<Object*>&);

        /**
         * @param output_html 
         */
        static void object_to_html(const Object&, StringBuilder& output_html, HashTable<Object*>&);

        /**
         * @param output_html 
         */
        static void function_to_html(const Object&, StringBuilder& output_html, HashTable<Object*>&);

        /**
         * @param output_html 
         */
        static void date_to_html(const Object&, StringBuilder& output_html, HashTable<Object*>&);

        /**
         * @param output_html 
         */
        static void error_to_html(const Object&, StringBuilder& output_html, HashTable<Object*>&);

        /**
         * @return String 
         */
        static String style_from_style_type(StyleType);

        /**
         * @return StyleType 
         */
        static StyleType style_type_for_token(Token);

        /**
         * @param type 
         * @return String 
         */
        static String open_style_type(StyleType type);

        /**
         * @param source 
         * @param type 
         * @return String 
         */
        static String wrap_string_in_style(String source, StyleType type);
    }; // class MarkupGenerator 
} // namespace JS