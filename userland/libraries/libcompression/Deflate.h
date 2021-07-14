/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Result.h>
#include <libcompression/Common.h>
#include <libio/BitWriter.h>
#include <libio/Reader.h>

namespace Compression
{

struct Deflate
{
private:
    unsigned int _compression_level;
    unsigned int _min_size_to_compress;

    static JResult compress_none(IO::Reader &uncompressed, IO::Writer &compressed);

    static JResult write_uncompressed_blocks(IO::Reader &in_data, IO::BitWriter &out_writer, bool final);
    static void write_block_header(IO::BitWriter &out_writer, BlockType block_type, bool final);
    static void write_uncompressed_block(const uint8_t *block_data, size_t block_len, IO::BitWriter &out_writer, bool final);

public:
    Deflate(unsigned int compression_level);

    JResult perform(IO::Reader &uncompressed, IO::Writer &compressed);
};

}