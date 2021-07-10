/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <math.h>
#include <libio/Scanner.h>
#include <libutils/Strings.h>

namespace IO
{

struct NumberScanner
{
    int _base = 10;

public:
    static NumberScanner binary() { return {2}; }

    static NumberScanner octal() {return {8}; }

    static NumberScanner decimal() { return {10}; }

    static NumberScanner hexadecimal() { return {16}; }

    bool is_digit(Scanner &scan)
    {
        char c = scan.peek();

        for (int i = 0; i < _base; i++)
        {
            if ((Strings::LOWERCASE_XDIGITS[i] == c) ||
                (Strings::UPPERCASE_XDIGITS[i] == c))
            {
                return true;
            }
        }

        return false;
    }

        Optional<uint8_t> scan_digit(Scanner &scan)
    {
        char c = scan.peek();

        for (int i = 0; i < _base; i++)
        {
            if ((Strings::LOWERCASE_XDIGITS[i] == c) ||
                (Strings::UPPERCASE_XDIGITS[i] == c))
            {
                scan.next();
                return i;
            }
        }

        return NONE;
    }

};

}