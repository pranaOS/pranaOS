/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/filesystem/FileDescription.h>
#include <kernel/filesystem/FileSystem.h>

namespace Kernel {

class FileBackedFileSystem : public FileSystem {
public:
    virtual ~FileBackedFileSystem() override;

    File& file() { return m_file_description->file(); }
    FileDescription& file_description() { return *m_file_description; }
    const File& file() const { return m_file_description->file(); }
    FileDescription& file_description() const { return *m_file_description; }

protected:
    explicit FileBackedFileSystem(FileDescription&);

private:
    virtual bool is_file_backed() const override { return true; }

    mutable NonnullRefPtr<FileDescription> m_file_description;
};

}