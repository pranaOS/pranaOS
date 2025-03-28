/**
 * @file sourcegenerator.h
 * @author Krisna Pranav
 * @brief sourcegenerator
 * @version 6.0
 * @date 2023-07-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/genericlexer.h>
#include <mods/hashmap.h>
#include <mods/string.h>
#include <mods/stringbuilder.h>

namespace Mods
{
    class SourceGenerator
    {
        MOD_MAKE_NONCOPYABLE(SourceGenerator);

    public:
        using MappingType = HashMap<StringView, String>;

        /**
         * @brief Construct a new Source Generator object
         * 
         * @param builder 
         * @param opening 
         * @param closing 
         */
        explicit SourceGenerator(StringBuilder& builder, char opening = '@', char closing = '@')
            : m_builder(builder), m_opening(opening), m_closing(closing)
        {
        }

        /**
         * @brief Construct a new Source Generator object
         * 
         * @param builder 
         * @param mapping 
         * @param opening 
         * @param closing 
         */
        explicit SourceGenerator(StringBuilder& builder, MappingType const& mapping, char opening = '@', char closing = '@')
            : m_builder(builder), m_mapping(mapping), m_opening(opening), m_closing(closing)
        {
        }   

        /**
         * @brief Construct a new Source Generator object
         * 
         */
        SourceGenerator(SourceGenerator&&) = default;

        /**
         * @return SourceGenerator 
         */
        SourceGenerator fork()
        {
            return SourceGenerator{m_builder, m_mapping, m_opening, m_closing};
        }

        /**
         * @param key 
         * @param value 
         */
        void set(StringView key, String value)
        {
            m_mapping.set(key, move(value));
        }

        /**
         * @param key 
         * @return String 
         */
        String get(StringView key) const
        {
            auto result = m_mapping.get(key);
            if(!result.has_value())
            {
                warnln("No key named `{}` set on SourceGenerator", key);
                VERIFY_NOT_REACHED();
            }
            return result.release_value();
        }

        /**
         * @return StringView 
         */
        StringView as_string_view() const
        {
            return m_builder.string_view();
        }

        /**
         * @return String 
         */
        String as_string() const
        {
            return m_builder.build();
        }

        /**
         * @param pattern 
         */
        void append(StringView pattern)
        {
            GenericLexer lexer{pattern};

            while(!lexer.is_eof())
            {
                auto const consume_until_without_consuming_stop_character = [&](char stop)
                {
                    return lexer.consume_while([&](char ch)
                                            { return ch != stop; });
                };

                m_builder.append(consume_until_without_consuming_stop_character(m_opening));

                if(lexer.consume_specific(m_opening))
                {
                    auto const placeholder = consume_until_without_consuming_stop_character(m_closing);

                    if(!lexer.consume_specific(m_closing))
                        VERIFY_NOT_REACHED();

                    m_builder.append(get(placeholder));
                }
                else
                {
                    VERIFY(lexer.is_eof());
                }
            }
        }

        /**
         * @param pattern 
         */
        void appendln(StringView pattern)
        {
            append(pattern);
            m_builder.append('\n');
        }

    private:
        StringBuilder& m_builder;
        MappingType m_mapping;
        char m_opening, m_closing;
    }; // class SourceGenerator
} // namespace Mods

using Mods::SourceGenerator;