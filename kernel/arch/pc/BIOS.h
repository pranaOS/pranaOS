/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <base/Vector.h>
#include <kernel/filesystem/SysFS.h>
#include <kernel/KBuffer.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/MappedROM.h>
#include <kernel/vm/Region.h>
#include <kernel/VirtualAddress.h>

namespace Kernel::SMBIOS {

struct [[gnu::packed]] LegacyEntryPoint32bit {
char legacy_sig[5];
    u8 checksum2;
    u16 smboios_table_length;
    u32 smbios_table_ptr;
    u16 smbios_tables_count;
    u8 smbios_bcd_revision;
};
struct [[gnu::packed]] EntryPoint32bit {
char sig[4];
    u8 checksum;
    u8 length;
    u8 major_version;
    u8 minor_version;
    u16 maximum_structure_size;
    u8 implementation_revision;
char formatted_area[5];
    LegacyEntryPoint32bit legacy_structure;
};
struct [[gnu::packed]] EntryPoint64bit {
char sig[5];
    u8 checksum;
    u8 length;
    u8 major_version;
    u8 minor_version;
    u8 document_revision;
    u8 revision;
    u8 reserved;
    u32 table_maximum_size;
    u64 table_ptr;
};
}

namespace Kernel {

MappedROM map_bios();
MappedROM map_ebda();

class BIOSSysFSComponent : public SysFSComponent {
public:
virtual KResultOr<size_t> read_bytes(off_t, size_t, UserOrKernelBuffer&, FileDescription*) const override;
protected:
virtual OwnPtr<KBuffer> try_to_generate_buffer() const = 0;
explicit BIOSSysFSComponent(String name);
};

}

}