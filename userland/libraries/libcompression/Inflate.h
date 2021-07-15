/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once
// 
#include <libabi/Result.h>
#include <libio/BitReader.h>
#include <libio/Read.h>
#include <libio/ReadCounter.h>
#include <libio/Writer.h>
#include <libutils/Assert.h>
#include <libutils/HashMap.h>
#include <libutils/Prelude.h>
#include <libutils/Vector.h>

namespace Compression
{

struct Inflate
{
private:
    Vector<unsigned int> _fixed_alphabet;
    Vector<unsigned int> _fixed_code_bit_lengths;
    Vector<unsigned int> _fixed_dist_alphabet;
    Vector<unsigned int> _fixed_dist_code_bit_lengths;

    Vector<unsigned int> _lit_len_alphabet;
    Vector<unsigned int> _lit_len_code_bit_length;
    Vector<unsigned int> _dist_code_bit_length;
    Vector<unsigned int> _dist_alphabet;

};

}