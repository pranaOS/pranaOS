//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once 

#include "virtualfilesystem.h"

namespace Kernel {
    class vfsManager {
    public:
        List<virtualFileSystem*>* Filesystems;

    public:
        vfsManager();

        int bootPartitionID = -1;

        void mount(virtualFileSystem* vfs);
        void unmount(virtualFileSystem* vfs);
        void unmountByDisk(Disk* disk);

        int extractDiskNumber(const char* path, uint8_t* idSizeReturn);
        bool searchBootPartition();

        int readFile(const char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
        int writeFile(const char* filename, uint8_t* buffer, uint32_t len, bool create = true);
        int createFile(const char* path);
        int createDirectory(const char* path);

        bool fileExists(const char* filename);
        bool directoryExists(const char* filename);
        bool removeFile(const char* filename);
        bool removeDirectory(const char* filename);
        bool ejectDrive(const char* path);

        uint32_t fileSize(const char* filename);
        List<Kernel::vfsEntry>* directoryList(const char* path);
    };
}