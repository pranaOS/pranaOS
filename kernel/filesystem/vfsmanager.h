//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include "virtualfilesystem.h"

namespace Kernel {
    class vfsManager {
    public:
        List<virtualFileSystem>* Filesystem;
    public:
        int bootPartitionID = -1;

        vfsManager();
        virtual ~vfsManager();

        void mount(virtualFileSystem* vfs);
        void unmount(virtualFileSystem* vfs);
        void unmountByDisk(Disk* disk);

        int extractDiskNumber(const char* path, ak::uint8_t* idSizeReturn);
        bool searchBootPartition();

        int readFile(const char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
        int writeFile(const char* filename, uint8_t* buffer, uint32_t len, bool create);
        bool fileExists(const char* filename);
        bool directoryExists(const char* filename);
    };
}