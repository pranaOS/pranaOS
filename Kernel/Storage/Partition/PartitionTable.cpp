// includes
#include <Kernel/Storage/Partition/PartitionTable.h>

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