/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libcompression/Deflate.h>
#include <libcompression/Inflate.h>
#include <libfile/ZipArchive.h>
#include <libio/Copy.h>
#include <libio/File.h>
#include <libio/MemoryReader.h>
#include <libio/MemoryWriter.h>
#include <libio/Read.h>
#include <libio/ScopedReader.h>
#include <libio/Skip.h>
#include <libio/Streams.h>
#include <libio/Write.h>
#include <libutils/Endian.h>

constexpr uint32_t ZIP_END_OF_CENTRAL_DIR_HEADER_SIG = 0x06054b50;
constexpr uint32_t ZIP_CENTRAL_DIR_HEADER_SIG = 0x02014b50;

constexpr uint32_t ZIP_LOCAL_DIR_HEADER_SIG = 0x04034b50;

constexpr uint32_t ZIP_CRC_MAGIC_NUMBER = 0xdebb20e3;

enum ExtraFieldType : uint16_t
{
    EFT_ZIP64 = 0x0001,
    EFT_EXTENDED_TIMESTAMP = 0x5455,
    EFT_NEW_UNIX = 0x7875,
};

using le_eft = LittleEndian<ExtraFieldType>;