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

    namespace
    {
        static constexpr size_t use_next_index = NumericLimits<size_t>;
        
        /**
         * @param value 
         * @param buffer 
         * @param base 
         * @param upper_case 
         * @return constexpr size_t 
         */
        static constexpr size_t convert_unsigned_to_string(u64 value, Array<u8, 128>& buffer, u8 base, bool upper_case)
        {
            VERIFY(base >= 2 && base <= 16);

            constexpr char const* lowercase_lookup = "0123456789abcdef";
            constexpr char const* uppercase_lookup = "0123456789ABCDEF";

            if (value == 0) {
                buffer[0] = '0';
                return 1;
            }

            size_t used = 0;
            while (value > 0) {
                if (upper_case)
                    buffer[used++] = uppercase_lookup[value % base];
                else
                    buffer[used++] = lowercase_lookup[value % base];

                value /= base;
            }

            for (size_t i = 0; i < used / 2; ++i)
                swap(buffer[i], buffer[used - i - 1]);

            return used;
        }
    } // namespace
} // namespace Mods