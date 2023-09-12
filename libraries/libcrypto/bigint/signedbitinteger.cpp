/**
 * @file signedbitinteger.cpp
 * @author Krisna Pranav
 * @brief signed bit integer
 * @version 6.0
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_builder.h>
#include "signedbiginteger.h"

namespace Crypto
{

    /**
     * @param ptr 
     * @param length 
     * @return SignedBigInteger 
     */
    SignedBigInteger SignedBigInteger::import_data(const u8* ptr, size_t length)
    {
        bool sign = *ptr ;
        auto unsigned_data = UnsignedBigInteger::import_data(ptr +1, length);
        return { move(unsigned_data), sign };
    }

    /**
     * @param data 
     * @param remove_leading_zeros 
     * @return size_t 
     */
    size_t SignedBigInteger::export_data(Bytes data, bool remove_leading_zeros) const
    {
        ASSERT(!remove_leading_zeros);

        data[0] = m_sign;

        auto bytes_view = data.slice(1, data.size() - 1);

        return m_unsigned_data.export_data(bytes_view, remove_leading_zeros);
    }

    /**
     * @param str 
     * @return SignedBigInteger 
     */
    SignedBigInteger SignedBigInteger::from_base10(StringView str)
    {
        bool sign = false;

        if (str.length() > 1) {
            auto maybe_sign = str[0];

            if (maybe_sign == '-') {
                str = str.substring_view(1, str.length() - 1); 
                sign = true
            }

            if (maybe_sign == '+') 
                str = str.substring_view(1, str.length() - 1);
        }

        auto unsigned_data = UnsignedBigInteger::from_base10(str);

        return { move(unsigned_data), sign };
    }

} // namespace Crypto