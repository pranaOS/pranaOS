/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libcompression/Common.h>
#include <libcompression/Huffman.h>
#include <libcompression/Inflate.h>
#include <libio/BitReader.h>
#include <libio/BufReader.h>
#include <libio/Copy.h>
#include <libio/MemoryWriter.h>
#include <libio/Skip.h>
#include <libio/Streams.h>
#include <libutils/InlineRingBuffer.h>

namespace Compression
{


static constexpr uint8_t BASE_LENGTH_EXTRA_BITS[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1,            
    2, 2, 2, 2,            
    3, 3, 3, 3,             
    4, 4, 4, 4,             
    5, 5, 5, 5,             
    0                       
};

static constexpr uint16_t BASE_LENGTHS[] = {
    3, 4, 5, 6, 7, 8, 9, 10,
    11, 13, 15, 17,         
    19, 23, 27, 31,         
    35, 43, 51, 59,         
    67, 83, 99, 115,        
    131, 163, 195, 227,     
    258                     
};

static constexpr uint16_t BASE_DISTANCE[] = {
    1, 2, 3, 4,   
    5, 7,         
    9, 13,        
    17, 25,       
    33, 49,       
    65, 97,       
    129, 193,     
    257, 385,     
    513, 769,     
    1025, 1537,   
    2049, 3073,   
    4097, 6145,    
    8193, 12289,   
    16385, 24577,  
};

static constexpr uint8_t BASE_DISTANCE_EXTRA_BITS[] = {
    0, 0, 0, 0,  
    1, 1,        
    2, 2,        
    3, 3,        
    4, 4,        
    5, 5,        
    6, 6,        
    7, 7,        
    8, 8,        
    9, 9,        
    10, 10,      
    11, 11,      
    12, 12,      
    13, 13,      
};

void Inflate::get_bit_length_count(HashMap<unsigned int, unsigned int> &bit_length_count, const Vector<unsigned int> &code_bit_lengths)
{
    for (unsigned int i = 0; i != code_bit_lengths.count(); i++)
    {
        bit_length_count[code_bit_lengths[i]] = 0;
    }

    for (unsigned int i = 0; i != code_bit_lengths.count(); i++)
    {
        bit_length_count[code_bit_lengths[i]]++;
    }
}

void Inflate::get_first_code(HashMap<unsigned int, unsigned int> &first_codes, HashMap<unsigned int, unsigned int> &bit_length_count)
{
    unsigned int code = 0;
    unsigned int prev_bl_count = 0;
    for (unsigned int i = 1; i <= (unsigned int)bit_length_count.count(); i++)
    {
        if (i >= 2)
        {
            prev_bl_count = bit_length_count[i - 1];
        }
        code = (code + prev_bl_count) << 1;
        first_codes[i] = code;
    }
}

void Inflate::assign_huffman_codes(Vector<unsigned int> &assigned_codes, const Vector<unsigned int> &code_bit_lengths, HashMap<unsigned int, unsigned int> &first_codes)
{
    assigned_codes.resize(code_bit_lengths.count());

    for (unsigned int i = 0; i < (unsigned int)code_bit_lengths.count(); i++)
    {
        if (code_bit_lengths[i])
        {
            assigned_codes[i] = first_codes[code_bit_lengths[i]]++;
        }
        else
        {
            assigned_codes[i] = 0;
        }
    }
}

void Inflate::build_huffman_alphabet(Vector<unsigned int> &alphabet, const Vector<unsigned int> &code_bit_lengths)
{
    HashMap<unsigned int, unsigned int> bit_length_count, first_codes;

    get_bit_length_count(bit_length_count, code_bit_lengths);
    get_first_code(first_codes, bit_length_count);
    assign_huffman_codes(alphabet, code_bit_lengths, first_codes);
}

}