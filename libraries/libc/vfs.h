#pragma once

#include <types.h>
#include <list/list.h>
#include <shared.h>

using namespace LibC;

namespace LibC {
    int readFile(char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
    int writeFile(char* filename, uint8_t* buffer, uint32_t len, bool create = true);

    bool fileExists(char* filename);
    bool dirExists(char* filename);

    int createFile(char* path);
    int createDirectory(char* path);

    uint32_t getFileSize(char* filename);
    list<vfsEntry> dirListing(char* path);

    bool ejectDisk(char* path);
}