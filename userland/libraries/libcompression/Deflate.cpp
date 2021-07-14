/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libcompression/Common.h>
#include <libcompression/Deflate.h>
#include <libcompression/Huffman.h>
#include <libio/BufReader.h>
#include <libutils/Array.h>

namespace Compression
{

Deflate::Deflate(unsigned int compression_level) : _compression_level(compression_level)
{
    _min_size_to_compress = 56 - (_compression_level * 4);
}

void Deflate::write_block_header(IO::BitWriter &out_writer, BlockType block_type, bool final)
{
    out_writer.put_bits(final ? 1 : 0, 1);
    out_writer.put_bits(block_type, 2);
}

}