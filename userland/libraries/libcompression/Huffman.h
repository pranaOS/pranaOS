/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/BitReader.h>

namespace Compression
{

struct HuffmanDecoder
{
private:
    const Vector<unsigned int> &_alphabet;
    const Vector<unsigned int> &_code_bit_lengths;
};

}