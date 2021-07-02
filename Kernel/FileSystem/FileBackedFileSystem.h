#pragma once

// includes
#include <Kernel/FileSystem/FileDescription.h>
#include <Kernel/FileSystem/FileSystem.h>

namespace Kernel {

class FileBackedFS : public FS {
public:
    virtual ~FileBackedFS() override;

    File& file() { return m_file_description->file(); }
    FileDescription& file_description() { return *m_file_description; }
    const File& file() const { return m_file_description->file(); }
    FileDescription& file_description() const { return *m_file_description; }

protected:
    explicit FileBackedFS(FileDescription&);

private:
    virtual bool is_file_backed() const override { return true; }

    mutable NonnullRefPtr<FileDescription> m_file_description;
};

}
