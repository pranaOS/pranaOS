/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/storage/partition/PartitionTable.h>

namespace Kernel {
PartitionTable::PartitionTable(const StorageDevice& device)
    : m_device(device)
{
}

Optional<DiskPartitionMetadata> PartitionTable::partition(unsigned index)
{
    if (index > partitions_count())
        return {};
    return m_partitions[index];
}

}
