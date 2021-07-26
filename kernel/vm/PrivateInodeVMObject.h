/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Bitmap.h>
#include <kernel/UnixTypes.h>
#include <kernel/VM/InodeVMObject.h>

namespace Kernel {

class PrivateInodeVMObject final : public InodeVMObject {
    BASE_MAKE_NONMOVABLE(PrivateInodeVMObject);

public:
    virtual ~PrivateInodeVMObject() override;

    static RefPtr<PrivateInodeVMObject> try_create_with_inode(Inode&);
    virtual RefPtr<VMObject> try_clone() override;

private:
    virtual bool is_private_inode() const override { return true; }

    explicit PrivateInodeVMObject(Inode&, size_t);
    explicit PrivateInodeVMObject(PrivateInodeVMObject const&);

    virtual StringView class_name() const override { return "PrivateInodeVMObject"sv; }

    PrivateInodeVMObject& operator=(PrivateInodeVMObject const&) = delete;
};

}
