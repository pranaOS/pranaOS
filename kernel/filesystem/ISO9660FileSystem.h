/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/EnumBits.h>
#include <base/HashMap.h>
#include <base/NonnullRefPtr.h>
#include <base/RecursionDecision.h>
#include <base/StringView.h>
#include <base/Types.h>
#include <kernel/filesystem/BlockBasedFileSystem.h>
#include <kernel/filesystem/Inode.h>
#include <kernel/KBuffer.h>
#include <kernel/KResult.h>

namespace Kernel {

namespace ISO {

template<typename T>
struct [[gnu::packed]] LittleAndBigEndian {
    T little;
    T big;
};

struct [[gnu::packed]] AsciiDateAndTime {
    u8 year[4];
    u8 month[2];
    u8 day[2];

    u8 hour[2];
    u8 minute[2];
    u8 second[2];
    u8 hundredths_of_second[2];

    u8 timezone_offset;
};
static_assert(sizeof(AsciiDateAndTime) == 17);

struct [[gnu::packed]] NumericalDateAndTime {
    u8 years_since_1900;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;

    u8 timezone_offset;
};
static_assert(sizeof(NumericalDateAndTime) == 7);


struct [[gnu::packed]] PathTableRecord {
    u8 directory_identifier_length;
    u8 extended_attribute_record_length;
    u32 extent_location;
    u16 parent_directory_number;

    u8 directory_identifier[];
};
static_assert(sizeof(PathTableRecord) == 8);

enum class ExtendedPermissions : u16 {
    SystemGroupReadable = 1 << 0,
    SystemGroupExecutable = 1 << 2,
    UserReadable = 1 << 4,
    UserExecutable = 1 << 6,
    GroupReadable = 1 << 8,
    GroupExecutable = 1 << 10,
    OtherReadable = 1 << 12,
    OtherExecutable = 1 << 14,
};
BASE_ENUM_BITWISE_OPERATORS(ExtendedPermissions);

enum class RecordFormat : u8 {
    NotSpecified = 0,
    FixedLengthRecords = 1,
    LittleEndianVariableRecords = 2,
    BigEndianVariableRecords = 3,
};

enum class RecordAttributes : u8 {

    LfCrDelimited = 0,
    FortranVerticalSpacing = 1,
    ContainsControlInformation = 2,
};

struct [[gnu::packed]] ExtendedAttributeRecord {
    LittleAndBigEndian<u16> owner_identification;
    LittleAndBigEndian<u16> group_identification;
    ExtendedPermissions permissions;

    AsciiDateAndTime file_creation_date_and_time;
    AsciiDateAndTime file_modification_date_and_time;
    AsciiDateAndTime file_expiration_date_and_time;
    AsciiDateAndTime file_effective_date_and_time;

    RecordFormat record_format;
    u8 record_attributes;

    LittleAndBigEndian<u16> record_length;

    u8 system_identifier[32];
    u8 system_use[64];

    u8 extended_attribute_record_version;
    u8 escape_sequence_length;

    u8 reserved[64];

    LittleAndBigEndian<u16> application_use_length;

    u8 application_use_and_escape_sequences[];
};
static_assert(sizeof(ExtendedAttributeRecord) == 250);

enum class FileFlags : u8 {
    Hidden = 1 << 0, 
    Directory = 1 << 1,
    AssociatedFile = 1 << 2,
    Record = 1 << 3,
    Protection = 1 << 4,
    MultiExtent = 1 << 7,
};

BASE_ENUM_BITWISE_OPERATORS(FileFlags);

struct [[gnu::packed]] DirectoryRecordHeader {
    u8 length;
    u8 extended_attribute_record_length;
    LittleAndBigEndian<u32> extent_location;
    LittleAndBigEndian<u32> data_length;
    NumericalDateAndTime recording_date_and_time;
    FileFlags file_flags;
    u8 file_unit_size;
    u8 interleave_gap_size;
    LittleAndBigEndian<u16> volume_sequence_number;
    u8 file_identifier_length;

};
static_assert(sizeof(DirectoryRecordHeader) == 33);

enum class VolumeDescriptorType : u8 {
    BootRecord = 0,
    PrimaryVolumeDescriptor = 1,
    SupplementaryOrEnhancedVolumeDescriptor = 2,
    VolumePartitionDescriptor = 3,
    VolumeDescriptorSetTerminator = 255,
};

struct [[gnu::packed]] VolumeDescriptorHeader {
    VolumeDescriptorType type;
    u8 identifier[5];
    u8 version;
};
static_assert(sizeof(VolumeDescriptorHeader) == 7);

struct [[gnu::packed]] BootRecord {
    VolumeDescriptorHeader header;
    u8 boot_system_identifier[32];
    u8 boot_identifier[32];
    u8 boot_system_use[1977];
};
static_assert(sizeof(BootRecord) == 2048);

struct [[gnu::packed]] VolumeDescriptorSetTerminator {
    VolumeDescriptorHeader header;
    u8 zeros[2041];
};
static_assert(sizeof(VolumeDescriptorSetTerminator) == 2048);

struct [[gnu::packed]] PrimaryVolumeDescriptor {
    VolumeDescriptorHeader header;
    u8 unused1;
    u8 system_identifier[32];
    u8 volume_identifier[32];
    u64 unused2;
    LittleAndBigEndian<u32> volume_space_size;
    u8 unused3[32];
    LittleAndBigEndian<u16> volume_set_size;
    LittleAndBigEndian<u16> volume_sequence_number;
    LittleAndBigEndian<u16> logical_block_size;
    LittleAndBigEndian<u32> path_table_size;

    u32 l_path_table_occurrence_location;
    u32 l_path_table_optional_occurrence_location;
    u32 m_path_table_occurrence_location;
    u32 m_path_table_optional_occurrence_location;

    DirectoryRecordHeader root_directory_record_header;
    u8 root_directory_identifier; 

    u8 volume_set_identifier[128];
    u8 publisher_identifier[128];
    u8 data_preparer_identifier[128];
    u8 application_identifier[128];

    u8 copyright_file_identifier[37];
    u8 abstract_file_identifier[37];
    u8 bibliographic_file_identifier[37];

    AsciiDateAndTime volume_creation_date_and_time;
    AsciiDateAndTime volume_modification_date_and_time;
    AsciiDateAndTime volume_expiration_date_and_time;
    AsciiDateAndTime volume_effective_date_and_time;

    u8 file_structure_version; 
    u8 unused4;
    u8 application_use[512];
    u8 reserved[653];
};
static_assert(sizeof(PrimaryVolumeDescriptor) == 2048);

struct [[gnu::packed]] VolumePartitionDescriptor {
    VolumeDescriptorHeader header;
    u8 unused;

    u8 system_identifier[32];
    u8 volume_partition_identifier[32];
    LittleAndBigEndian<u32> volume_partition_location;
    LittleAndBigEndian<u32> volume_partition_size;

    u8 system_use[1960];
};
static_assert(sizeof(VolumePartitionDescriptor) == 2048);

}

class ISO9660Inode;
class ISO9660DirectoryIterator;

class ISO9660FS final : public BlockBasedFileSystem {
    friend ISO9660Inode;
    friend ISO9660DirectoryIterator;

public:
    struct DirectoryEntry : public RefCounted<DirectoryEntry> {
        u32 extent { 0 };
        u32 length { 0 };

        OwnPtr<KBuffer> blocks;

        static KResultOr<NonnullRefPtr<DirectoryEntry>> try_create(u32 extent, u32 length, OwnPtr<KBuffer> blocks)
        {
            auto result = adopt_ref_if_nonnull(new (nothrow) DirectoryEntry(extent, length, move(blocks)));
            if (!result) {
                return ENOMEM;
            }
            return result.release_nonnull();
        }

    private:
        DirectoryEntry(u32 extent, u32 length, OwnPtr<KBuffer> blocks)
            : extent(extent)
            , length(length)
            , blocks(move(blocks))
        {
        }
    };

    static KResultOr<NonnullRefPtr<ISO9660FS>> try_create(FileDescription&);

    virtual ~ISO9660FS() override;
    virtual bool initialize() override;
    virtual StringView class_name() const override { return "ISO9660FS"; }
    virtual Inode& root_inode() override;

    virtual unsigned total_block_count() const override;
    virtual unsigned total_inode_count() const override;

    virtual u8 internal_file_type_to_directory_entry_type(DirectoryEntryView const& entry) const override;

    KResultOr<NonnullRefPtr<DirectoryEntry>> directory_entry_for_record(Badge<ISO9660DirectoryIterator>, ISO::DirectoryRecordHeader const* record);

private:
    ISO9660FS(FileDescription&);

    KResult parse_volume_set();
    KResult create_root_inode();
    KResult calculate_inode_count() const;

    u32 calculate_directory_entry_cache_key(ISO::DirectoryRecordHeader const&);

    KResult visit_directory_record(ISO::DirectoryRecordHeader const& record, Function<KResultOr<RecursionDecision>(ISO::DirectoryRecordHeader const*)> const& visitor) const;

    OwnPtr<ISO::PrimaryVolumeDescriptor> m_primary_volume;
    RefPtr<ISO9660Inode> m_root_inode;

    mutable u32 m_cached_inode_count { 0 };
    HashMap<u32, NonnullRefPtr<DirectoryEntry>> m_directory_entry_cache;
};

class ISO9660Inode final : public Inode {
    friend ISO9660FS;

public:
    virtual ~ISO9660Inode() override;

    virtual KResultOr<size_t> read_bytes(off_t, size_t, UserOrKernelBuffer& buffer, FileDescription*) const override;
    virtual InodeMetadata metadata() const override;
    virtual KResult traverse_as_directory(Function<bool(FileSystem::DirectoryEntryView const&)>) const override;
    virtual RefPtr<Inode> lookup(StringView name) override;
    virtual void flush_metadata() override;
    virtual KResultOr<size_t> write_bytes(off_t, size_t, const UserOrKernelBuffer& buffer, FileDescription*) override;
    virtual KResultOr<NonnullRefPtr<Inode>> create_child(StringView name, mode_t, dev_t, uid_t, gid_t) override;
    virtual KResult add_child(Inode&, const StringView& name, mode_t) override;
    virtual KResult remove_child(const StringView& name) override;
    virtual KResult chmod(mode_t) override;
    virtual KResult chown(uid_t, gid_t) override;
    virtual KResult truncate(u64) override;
    virtual KResult set_atime(time_t) override;
    virtual KResult set_ctime(time_t) override;
    virtual KResult set_mtime(time_t) override;
    virtual void one_ref_left() override;

private:

    static constexpr size_t max_file_identifier_length = 256 - sizeof(ISO::DirectoryRecordHeader);

    ISO9660Inode(ISO9660FS&, ISO::DirectoryRecordHeader const& record, StringView const& name);
    static KResultOr<NonnullRefPtr<ISO9660Inode>> try_create_from_directory_record(ISO9660FS&, ISO::DirectoryRecordHeader const& record, StringView const& name);

    static InodeIndex get_inode_index(ISO::DirectoryRecordHeader const& record, StringView const& name);
    static StringView get_normalized_filename(ISO::DirectoryRecordHeader const& record, Bytes buffer);

    void create_metadata();
    time_t parse_numerical_date_time(ISO::NumericalDateAndTime const&);

    InodeMetadata m_metadata;
    ISO::DirectoryRecordHeader m_record;
};

}

using Kernel::ISO::has_any_flag;
using Kernel::ISO::has_flag;
