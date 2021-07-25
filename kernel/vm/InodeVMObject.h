/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Bitmap.h>
#include <kernel/UnixTypes.h>
#include <kernel/vm/VMObject.h>

namespace Kernel {

class InodeVMObject : public VMObject {
public:
    virtual ~InodeVMObject() override;

    Inode& inode() { return *m_inode; }
    Inode const& inode() const { return *m_inode; }

    size_t amount_dirty() const;
    size_t amount_clean() const;

    int release_all_clean_pages();

    u32 writable_mappings() const;
    u32 executable_mappings() const;

protected:
    explicit InodeVMObject(Inode&, size_t);
    explicit InodeVMObject(InodeVMObject const&);

    InodeVMObject& operator=(InodeVMObject const&) = delete;
    InodeVMObject& operator=(InodeVMObject&&) = delete;
    InodeVMObject(InodeVMObject&&) = delete;

    virtual bool is_inode() const final { return true; }

    NonnullRefPtr<Inode> m_inode;
    Bitmap m_dirty_pages;
};

}