/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Compression
{

enum BlockType
{
    BT_UNCOMPRESSED = 0,
    BT_FIXED_HUFFMAN = 1,
    BT_DYNAMIC_HUFFMAN = 2,
};

}