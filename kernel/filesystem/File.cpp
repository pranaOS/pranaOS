/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <base/Userspace.h>
#include <kernel/filesystem/File.h>
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Process.h>

namespace Kernel {

File::File()
{
}

File::~File()
{
}

KResultOr<NonnullRefPtr<FileDescription>> File::open(int options)
{
    auto description = FileDescription::create(*this);
    if (!description.is_error()) {
        description.value()->set_rw_mode(options);
        description.value()->set_file_flags(options);
    }
    return description;
}

KResult File::close()
{
    return KSuccess;
}

KResult File::ioctl(FileDescription&, unsigned, Userspace<void*>)
{
    return ENOTTY;
}

KResultOr<Region*> File::mmap(Process&, FileDescription&, const Range&, u64, int, bool)
{
    return ENODEV;
}

KResult File::attach(FileDescription&)
{
    m_attach_count++;
    return KSuccess;
}

void File::detach(FileDescription&)
{
    m_attach_count--;
}
}