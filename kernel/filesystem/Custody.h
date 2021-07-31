/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/String.h>
#include <kernel/Forward.h>
#include <kernel/heap/SlabAllocator.h>
#include <kernel/KResult.h>
#include <kernel/KString.h>

namespace Kernel {


class Custody : public RefCounted<Custody> {
    MAKE_SLAB_ALLOCATED(Custody)
public:
    static KResultOr<NonnullRefPtr<Custody>> try_create(Custody* parent, StringView name, Inode&, int mount_flags);

    ~Custody();

    Custody* parent() { return m_parent.ptr(); }
    Custody const* parent() const { return m_parent.ptr(); }
    Inode& inode() { return *m_inode; }
    Inode const& inode() const { return *m_inode; }
    StringView name() const { return m_name->view(); }
    OwnPtr<KString> try_create_absolute_path() const;
    String absolute_path() const;

    int mount_flags() const { return m_mount_flags; }
    bool is_readonly() const;

private:
    Custody(Custody* parent, NonnullOwnPtr<KString> name, Inode&, int mount_flags);

    RefPtr<Custody> m_parent;
    NonnullOwnPtr<KString> m_name;
    NonnullRefPtr<Inode> m_inode;
    int m_mount_flags { 0 };
};

}