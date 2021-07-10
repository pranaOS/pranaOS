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
}

}