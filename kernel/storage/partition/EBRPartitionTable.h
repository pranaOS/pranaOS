/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullOwnPtr.h>
#include <base/RefPtr.h>
#include <base/Result.h>
#include <base/Vector.h>
#include <kernel/storage/partition/DiskPartition.h>
#include <kernel/storage/partition/MBRPartitionTable.h>

namespace Kernel {

struct EBRPartitionHeader;
class EBRPartitionTable : public MBRPartitionTable {
public:
    ~EBRPartitionTable();

    static Result<NonnullOwnPtr<EBRPartitionTable>, PartitionTable::Error> try_to_initialize(const StorageDevice&);
    explicit EBRPartitionTable(const StorageDevice&);
    virtual bool is_valid() const override { return m_valid; };

private:
    void search_extended_partition(const StorageDevice&, MBRPartitionTable&, u64, size_t limit);

    bool m_valid { false };
};
}
