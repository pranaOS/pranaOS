/**
 * @file format.cpp
 * @author Krisna Pranav
 * @brief format
 * @version 6.0
 * @date 2025-02-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/charactertypes.h>
#include <mods/format.h>
#include <mods/genericlexer.h>
#include <mods/integralmath.h>
#include <mods/stringbuilder.h>
#include <mods/kstdio.h>

#if defined(__pranaos__) && !defined(KERNEL)
#   include <pranaos.h>
#endif

namespace Mods
{
    class FormatParser : public GenericLexer
    {
    public:
        struct FormatSpecifier
        {
            StringView flags;
            size_t index;
        }; // struct FormatSpecifier

        /**
         * @brief Construct a new Format Parser object
         * 
         * @param input 
         */
        explicit FormatParser(StringView input);

        StringView consume_literal();

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool consume_number(size_t& value);

        /**
         * @param specifier 
         * @return true 
         * @return false 
         */
        bool consume_specifier(FormatSpecifier& specifier);

        /**
         * @param index 
         * @return true 
         * @return false 
         */
        bool consume_replacement_field(size_t& index);
    }; // class FormatParser : public GenericLexer
} // namespace Mods