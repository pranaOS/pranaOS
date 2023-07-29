/**
 * @file sourcegenerator.h
 * @author Krisna Pranav
 * @brief sourcegenerator
 * @version 1.0
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "genericlexer.h"
#include "hashmap.h"
#include "string.h"
#include "string_builder.h"

namespace Mods
{

class SourceGenerator 
{
    MOD_MAKE_NONCOPYABLE(SourceGenerator);

public:
    using MappingType = HashMap<StringView, String>;

    /**
     * @param builder 
     * @param opening 
     * @param closing 
     */
    explicit SourceGenerator(StringBuilder& builder, char opening = '@', char closing = '@')
        : m_builder(builder)
        , m_opening(opening)
        , m_closing(closing)
    {}

    /**
     * @return SourceGenerator 
     */
    SourceGenerator fork()
    {
        return SourceGenerator { m_builder, m_mapping };
    };

    /**
     * @param key 
     * @param value 
     */
    void set(StringView key, String value) 
    {
        m_mapping.set(key, value);
    }

    /**
     * @param key 
     * @return String 
     */
    String get(StringView key) const
    {
        return m_mapping.get(key).value;
    }

    /**
     * @return StringView 
     */
    StringView as_string_view() const
    {
        return m_builder.string_view();
    };

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
        GenericLexer lexer { pattern };

        while (!lexer.is_eof()) {
            const auto consume_until_without_stop_character = [&](char stop) {
                return lexer.consume_while([&](char ch) { return ch != stop});
            }

            if (lexer.consume_specifi(m_opening)) {
                const auto placeholder = consume_until_without_stop_character();

                if (!lexer.consume_specific(m_closing))
                    ASSERT_NOT_REACHED();
                
                m_builder.append(get(placeholder));
            } else {
                ASSERT(lexer.is_eof());
            }
        }
    }

private:
    StringBuilder& m_builder;
    MappingType m_mapping;
    char m_opening, m_closing;
}; // class SourceGenerator

} // namespace Mods