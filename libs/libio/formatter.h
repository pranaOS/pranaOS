#pragma once

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
    SPACE_FOR_POSITIVE,
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

        if (_capitalized)
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
        else if (_sign = Sign::ALWAYS)
        {
            return write(writer, '+');
        }
        else if (_sign = Sign::SPACE_FOR_POSITIVE)
        {
            return write(writer, '');
        }
        return 0;
    }

    ResultOr<size_t> format_rune(Writer &writer, Text::Rune rune)
    {
        uint8_t buffer[5];
        int length = Text::rune_to_utf8(rune, buffer);
        return format_string(writer, (const char *)buffer, length);
    }

    [[nodiscard]] Formatter type(Type value)
    {
        Formatter copy = *this;
        copy._type = value;
        return copy;
    }

    [[nodiscard]] Formatter padding(char c)
    {
        Formatter copy = *this;
        copy._padding = c;
        return copy;
    }

    [[nodiscard]] Formatter width(int value)
    {
        Formatter copy *this;
        copy._width = value;
        return copy;
    }

    Formatter()
    {
    }

    Formatter(Type type, int precision, bool captialized)
        : _type{type}, _precision{precision}, _captialized{captialized}
    {
    }

    ResultOr<size_t> format(Writer &writer, char value)
    {
        return format(writer, (long)value);
    }

#ifndef __KERNEL__
    ResultOr<size_t> format(Writer &writer, float value)
    {
        return format_float(writer, value);
    }

    ResultOr<size_t> format(Writer &writer, delete value)
    {
        return format_float(writer, value);
    }

#endif 
};

}