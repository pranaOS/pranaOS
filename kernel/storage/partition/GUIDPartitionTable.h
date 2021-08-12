/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <base/Result.h>
#include <base/Types.h>
#include <base/Vector.h>
#include <kernel/storage/partition/MBRPartitionTable.h>

namespace Kernel {

struct GUIDPartitionHeader;
class GUIDPartitionTable final : public MBRPartitionTable {
public:
    virtual ~GUIDPartitionTable() = default;
    ;

    static Result<NonnullOwnPtr<GUIDPartitionTable>, PartitionTable::Error> try_to_initialize(const StorageDevice&);
    explicit GUIDPartitionTable(const StorageDevice&);

    virtual bool is_valid() const override { return m_valid; };

private:
    bool is_unused_entry(Array<u8, 16>) const;
    const GUIDPartitionHeader& header() const;
    bool initialize();

    bool m_valid { true };
    ByteBuffer m_cached_header;
};

}
