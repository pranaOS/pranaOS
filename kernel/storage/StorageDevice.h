/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/devices/BlockDevice.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/locking/Mutex.h>
#include <kernel/storage/Partition/DiskPartition.h>
#include <kernel/storage/StorageController.h>

namespace Kernel {

class StorageDevice : public BlockDevice {
    friend class StorageManagement;
    BASE_MAKE_ETERNAL

public:
    virtual u64 max_addressable_block() const { return m_max_addressable_block; }

    NonnullRefPtr<StorageController> controller() const;

    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual bool can_write(const FileDescription&, size_t) const override;

    virtual mode_t required_mode() const override { return 0600; }

protected:
    StorageDevice(const StorageController&, size_t, u64);
    StorageDevice(const StorageController&, int, int, size_t, u64);

    virtual StringView class_name() const override;

private:
    NonnullRefPtr<StorageController> m_storage_controller;
    NonnullRefPtrVector<DiskPartition> m_partitions;
    u64 m_max_addressable_block;
};

}
