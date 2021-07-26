/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Bitmap.h>
#include <kernel/UnixTypes.h>
#include <kernel/vm/InodeVMObject.h>

namespace Kernel {

class SharedInodeVMObject final : public InodeVMObject {
    BASE_MAKE_NONMOVABLE(SharedInodeVMObject);

public:
    static RefPtr<SharedInodeVMObject> try_create_with_inode(Inode&);

private:
    virtual bool is_shared_inode() const override { return true; }

    explicit SharedInodeVMObject(Inode&, size_t);

    SharedInodeVMObject& operator=(SharedInodeVMObject const&) = delete;

};

}