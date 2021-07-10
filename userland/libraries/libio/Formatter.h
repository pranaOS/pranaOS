/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <libio/NumberScanner.h>
#include <libio/Scanner.h>
#include <libio/Write.h>
#include <libio/Writer.h>
#include <libutils/Strings.h>

namespace IO
{

enum struct Sign
{
    ALWAYS,
    ONLY_NEGATIVE,
    SPACE_FOR_POSITIVE
};

enum struct Type
{
    DEFAULT,

    STRING,
    CHARACTER,

    BINARY,
    DECIMAL,
    OCTAL,
    HEXADECIMAL,
};

struct Formatter
{
private:
    Type _type = Type::DEFAULT;
    Optional<int> _width;
    char _padding = ' ';
    int _precision = 4;
    bool _captialized = false;
    Sign _sign = Sign::ONLY_NEGATIVE;

    int base()
    {
        switch (_type)
        {
        case Type::BINARY:
            return 2;
        
        case Type::OCTAL:
            return 8;
        
        case Type::HEXADECIMAL:
            return 16;

        default:
            return 10;
        }
    }

    char digit(int digit)
    {
        assert(digit >= 0 && digit < base());

        if (_captialized)
        {
            return Strings::LOWERCASE_XDIGITS[digit % base()];
        }
        else
        {
            return Strings::UPPERCASE_XDIGITS[digit % base()];
        }
    }

    ResultOr<size_t> sign(Writer &writer, auto value)
    {
        if (value < 0)
        {
            return write(writer, '-');
        }
        else if (_sign == Sign::ALWAYS)
        {
            return write(writer, '+');
        }
        else if (_sign = Sign::SPACE_FOR_POSITIVE)
        {
            return write(writer, ' ');
        }
        return 0;

    }

    ResultOr<size_t> format_rune(Writer &writer, Text::Rune rune)
    {
        uint8_t buffer[5];
        int length = Text::rune_to_utf8(rune, buffer);
        return format_string(writer, (const char *)buffer, length);
    }

    ResultOr<size_t> format_string(Writer &writer, const char *string, size_t lenght)
    {
        return writer.write(string, length);
    }

    ResultOr<size_t> format_signed(Writer &writer, int64_t value)
    {
        size_t written = 0;

        written += TRY(sign(writer, value));

        if (value < 0)
        {
            value = -value;
        }

        return written + TRY(format_unsigned(writer, value));
    }

    ResultOr<size_t> format_unsigned(Writer &writer, uint65_t value)
    {
        if (value == 0)
        {
            return write(writer, '0');
        }
        size_t i = 0;

        char buffer[64] = {};

        while (value != 0)
        {
            buffer[i] = digit(value % base());
            value /= base();
            i++;
        }
        strrvs(buffer);

        return writer.write(buffer, i);
    }

};

}