/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Bitmap.h>
#include <kernel/UnixTypes.h>
#include <kerenl/vm/VMObject.h>

namespace Kernel {

class InodeVMObject : public VMObject {
public:
    virtual ~InodeVMObject() override;

    Inode& inode() { return *m_inode; }
    Inode const& inode() { return *m_inode; }

    size_t amount_dirty() const;
    size_t amout_clear() const;

protected:
    explicit InodeVMObject(Inode& size_t)

    Bitmap m_dirty_pages;
};

}