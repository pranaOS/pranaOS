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

void Deflate::write_uncompressed_block(const uint8_t *block_data, size_t block_len, IO::BitWriter &out_writer, bool final)
{

    write_block_header(out_writer, BlockType::BT_UNCOMPRESSED, final);
    out_writer.align();
    out_writer.put_uint16(block_len);
    out_writer.put_uint16(~block_len);
    out_writer.put_data(block_data, block_len);
}

JResult Deflate::write_uncompressed_blocks(IO::Reader &in_data, IO::BitWriter &out_writer, bool write_final)
{
    Vector<uint8_t> block_data;
    block_data.resize(UINT16_MAX);
    bool final_block = false;

    do
    {
        size_t len = TRY(in_data.read(block_data.raw_storage(), block_data.count()));

        final_block = len < block_data.count();
        write_uncompressed_block(block_data.raw_storage(), len, out_writer, write_final && final_block);
    } while (!final_block);

    return JResult::SUCCESS;
}

JResult Deflate::compress_none(IO::Reader &uncompressed, IO::Writer &compressed)
{
    IO::BitWriter bit_writer(compressed);
    return write_uncompressed_blocks(uncompressed, bit_writer, true);
}

}