/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/filesystem/Custody.h>
#include <kernel/filesystem/FileSystem.h>
#include <kernel/filesystem/Inode.h>
#include <kernel/filesystem/Mount.h>

namespace Kernel {

Mount::Mount(FileSystem& guest_fs, Custody* host_custody, int flags)
    : m_guest(guest_fs.root_inode())
    , m_guest_fs(guest_fs)
    , m_host_custody(host_custody)
    , m_flags(flags)
{
}

Mount::Mount(Inode& source, Custody& host_custody, int flags)
    : m_guest(source)
    , m_guest_fs(source.fs())
    , m_host_custody(host_custody)
    , m_flags(flags)
{
}

String Mount::absolute_path() const
{
    if (!m_host_custody)
        return "/";
    return m_host_custody->absolute_path();
}

Inode* Mount::host()
{
    if (!m_host_custody)
        return nullptr;
    return &m_host_custody->inode();
}

Inode const* Mount::host() const
{
    if (!m_host_custody)
        return nullptr;
    return &m_host_custody->inode();
}

}
