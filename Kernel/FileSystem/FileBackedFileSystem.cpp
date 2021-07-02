// includes
#include <Kernel/FileSystem/FileBackedFileSystem.h>

namespace Kernel {

FileBackedFS::FileBackedFS(FileDescription& file_description)
    : m_file_description(file_description)
{
}

FileBackedFS::~FileBackedFS()
{
}

}
