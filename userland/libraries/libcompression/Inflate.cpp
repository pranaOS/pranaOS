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

}