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

using le_eft = LittleEndian<ExtraFieldType>;

enum EntryFlags : uint16_t
{
    EF_NONE = 0,
    EF_ENCRYPTED = 1 << 0,
    EF_COMPRESSION_OPTION = 1 << 1,
    EF_COMPRESSION_OPTION2 = 1 << 2,
    EF_DATA_DESCRIPTOR = 1 << 3,
};

using le_flags = LittleEndian<EntryFlags>;

enum CompressionMethod : uint16_t
{
    CM_UNCOMPRESSED = 0,
    CM_SHRUNK = 1,
    CM_DEFLATED = 8,
};

using le_compression = LittleEndian<CompressionMethod>;

struct PACKED CentralDirectoryFileHeader
{
    le_uint32_t signature;
    le_uint16_t version;
    le_uint16_t version_required;
    le_flags flags;
    le_compression compression;
    le_uint16_t mod_time;
    le_uint16_t mod_date;
    le_uint32_t crc;
    le_uint32_t compressed_size;
    le_uint32_t uncompressed_size;
    le_uint16_t len_filename;
    le_uint16_t len_extrafield;
    le_uint16_t len_comment;
    le_uint16_t disk_start;
    le_uint16_t internal_attr;
    le_uint32_t external_attr;
    le_uint32_t local_header_offset;
};

struct PACKED CentralDirectoryEndRecord
{
    le_uint32_t signature;
    le_uint16_t disk1;
    le_uint16_t disk2;
    le_uint16_t disk_entries;
    le_uint16_t total_entries;
    le_uint32_t central_dir_size;
    le_uint32_t central_dir_offset;
    le_uint16_t len_comment;
};

struct PACKED LocalHeader
{
    le_uint32_t signature;
    le_uint16_t version;
    le_flags flags;
    le_compression compression;
    le_uint16_t mod_time;
    le_uint16_t mod_date;
    le_uint32_t crc;
    le_uint32_t compressed_size;
    le_uint32_t uncompressed_size;
    le_uint16_t len_filename;
    le_uint16_t len_extrafield;
};

static_assert(sizeof(LocalHeader) == 30, "LocalHeader has invalid size!");

struct PACKED DataDescriptor
{
    le_uint32_t crc;
    le_uint32_t compressed_size;
    le_uint32_t uncompressed_size;
};

static_assert(sizeof(DataDescriptor) == 12, "DataDescriptor has invalid size!");

ZipArchive::ZipArchive(IO::Path path, bool read) : Archive(path)
{
    if (read)
    {
        IO::logln("Did read archive, result: {}", read_archive());
    }
}

JResult read_local_headers(IO::SeekableReader auto &reader, Vector<Archive::Entry> &entries)
{
    while (TRY(reader.tell()) < (TRY(reader.length()) - sizeof(LocalHeader)))
    {
        auto local_header = TRY(IO::peek<LocalHeader>(reader));

        if (local_header.signature() != ZIP_LOCAL_DIR_HEADER_SIG)
        {
            break;
        }

        auto &entry = entries.emplace_back();
        Assert::equal(IO::skip(reader, sizeof(LocalHeader)), SUCCESS);

        // Get the uncompressed & compressed sizes
        entry.uncompressed_size = local_header.uncompressed_size();
        entry.compressed_size = local_header.compressed_size();
        entry.compression = local_header.compression();

        // Read the filename of this entry
        entry.name = TRY(IO::read_string(reader, local_header.len_filename()));

        // Read extra fields
        auto end_position = TRY(reader.tell()) + local_header.len_extrafield();
        while (TRY(reader.tell()) < end_position)
        {
            le_eft extra_field_type = TRY(IO::read<ExtraFieldType>(reader));
            le_uint16_t extra_field_size = TRY(IO::read<uint16_t>(reader));

            UNUSED(extra_field_type);
            TRY(IO::skip(reader, extra_field_size()));
        }

        entry.archive_offset = TRY(reader.tell());
        TRY(IO::skip(reader, entry.compressed_size));

        if (local_header.flags() & EF_DATA_DESCRIPTOR)
        {
            auto data_descriptor = TRY(IO::read<DataDescriptor>(reader));
            entry.uncompressed_size = data_descriptor.uncompressed_size();
            entry.compressed_size = data_descriptor.compressed_size();
        }
    }

    return JResult::SUCCESS;
}

JResult read_central_directory(IO::SeekableReader auto &reader)
{

    while (TRY(reader.tell()) < (TRY(reader.length()) - sizeof(CentralDirectoryFileHeader)))
    {
        IO::logln("Read central directory header");
        auto cd_file_header = TRY(IO::peek<CentralDirectoryFileHeader>(reader));

        if (cd_file_header.signature() != ZIP_CENTRAL_DIR_HEADER_SIG)
        {
            break;
        }

        TRY(IO::skip(reader, sizeof(CentralDirectoryFileHeader)));

        String name = TRY(IO::read_string(reader, cd_file_header.len_filename()));
        IO::logln("Found central directory header: '{}'", name);

        TRY(IO::skip(reader, cd_file_header.len_extrafield()));
        TRY(IO::skip(reader, cd_file_header.len_comment()));
    }

    le_uint32_t central_dir_end_sig = TRY(IO::read<uint32_t>(reader));
    if (central_dir_end_sig() != ZIP_END_OF_CENTRAL_DIR_HEADER_SIG)
    {
        IO::logln("Missing 'central directory end record' signature!");
        return ERR_INVALID_DATA;
    }

    return JResult::SUCCESS;
}

JResult ZipArchive::read_archive()
{
    _valid = false;

    IO::File archive_file(_path, J_OPEN_READ);

    if (!archive_file.exist())
    {
        IO::logln("Archive does not exist: {}", _path.string());
        return ERR_NO_SUCH_FILE_OR_DIRECTORY;
    }

    IO::logln("Opening file: '{}'", _path.string());

    if (TRY(archive_file.length()) < sizeof(CentralDirectoryEndRecord))
    {
        IO::logln("Archive is too small to be a valid .zip: {} {}", _path.string(), archive_file.length().unwrap());
        return ERR_INVALID_DATA;
    }

    TRY(read_local_headers(archive_file, _entries));
    TRY(read_central_directory(archive_file));

    _valid = true;
    return JResult::SUCCESS;
}