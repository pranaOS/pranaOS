//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include <ak/types>
#include <ak/list.h>
#include <kernel/disks/disk.h>
#include <libc/shared.h>

namespace Kernel {
    #define PATH_SEPERATOR_C '\\'
    #define PATH_SEPERATOR_S "\\"

    class virtualFileSystem {
        friend class virtualFileSystemManager;
      public:
        Disk* disk;

      public:
        virtualFileSystem(Disk* disk, ak::uint32_t start, ak::uint32_t size, char* name = 0);
        virtual ~virtualFileSystem();

        virtual int readFile(const char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
        virtual int writeFile(const char* filename, uint8_t* buffer, uint32_t len, bool create = true);
        virtual int createFile(const char* path);
        virtual int createDirectory(const char* path);

        virtual bool fileExists(const char* filename);
        virtual bool directoryExists(const char* filename);

        virtual bool initialize();

      protected:
        ak::uint32_t startLBA;
        ak::uint32_t sizeInSecotrs;

        char* name = "Unknown";
    };
}