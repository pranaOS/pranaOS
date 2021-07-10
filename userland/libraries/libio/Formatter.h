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
};

}