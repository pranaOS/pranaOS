/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <base/Vector.h>
#include <kernel/storage/partition/DiskPartition.h>
#include <kernel/storage/partition/DiskPartitionMetadata.h>
#include <kernel/storage/StorageDevice.h>

namespace Kernel {

class PartitionTable {
public:
    enum class Error {
        Invalid,
        MBRProtective,
        ConatinsEBR,
    };

public:
    Optional<DiskPartitionMetadata> partition(unsigned index);
    size_t partitions_count() const { return m_partitions.size(); }
    virtual ~PartitionTable() = default;
    virtual bool is_valid() const = 0;

    Vector<DiskPartitionMetadata> partitions() const { return m_partitions; }

protected:
    explicit PartitionTable(const StorageDevice&);

    NonnullRefPtr<StorageDevice> m_device;
    Vector<DiskPartitionMetadata> m_partitions;
};

}