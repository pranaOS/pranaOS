/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Bitmap.h>
#include <kernel/memory/InodeVMObject.h>
#include <kernel/UnixTypes.h>

namespace Kernel::Memory {

class SharedInodeVMObject final : public InodeVMObject {
    BASE_MAKE_NONMOVABLE(SharedInodeVMObject);

public:
    static RefPtr<SharedInodeVMObject> try_create_with_inode(Inode&);
    virtual RefPtr<VMObject> try_clone() override;

private:
    virtual bool is_shared_inode() const override { return true; }

    explicit SharedInodeVMObject(Inode&, size_t);
    explicit SharedInodeVMObject(SharedInodeVMObject const&);

    virtual StringView class_name() const override { return "SharedInodeVMObject"sv; }

    SharedInodeVMObject& operator=(SharedInodeVMObject const&) = delete;
};

}
