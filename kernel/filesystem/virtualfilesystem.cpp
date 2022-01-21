//
// Created by KrisnaPranav on 21/01/22.
//

#include "virtualfilesystem.h"
#include <kernel/system/log.h>

using Kernel;

Kernel::virtualFileSystem::virtualFileSystem(Disk* disk, ak::uint32_t start, ak::uint32_t size, char* name) {
    this->disk = disk;
    this->sizeInSectors = size;
    this->startLBA = start;
    this->Name = name;
}

Kernel::virtualFileSystem::~virtualFileSystem() {
    Log(Error, "filesystem failed %s%d", __FILE__, __LINE__);
}

bool Kernel::virtualFileSystem::initialize() {
    return false;
}

int Kernel::virtualFileSystem::readFile(const char* filename, int* buffer, int offset, int len) {
    Log(Error, "virtual filesystem failed %s%d", __FILE__, __LINE__);
    return -1;
}

int Kernel::virtualFileSystem::writeFile(const char* filename, int* buffer, int len, bool create) {
    Log(Error, "virtual filesystem failed %s%d", __FILE__, __LINE__);
}

bool Kernel::virtualFileSystem::fileExists(const char* filename) {
    Log(Error, "virtual filesystem failed %s:%d", __FILE__, __LINE__);

    return false;
}

bool Kernel::virtualFileSystem::directoryExists(const char* filename) {
    Log(Error, "virtual filesystem failed %s:%d", __FILE__, __LINE__);

    return false;
}

int Kernel::virtualFileSystem::createFile(const char* path) {
    Log(Error, "virtual filesystem failed %s:%d", __FILE__, __LINE__);
    return -1;
}

int Kernel::virtualFileSystem::createDirectory(const char* path) {
    Log(Error, "virtaul filesystem failed %s:%d", __FILE__, __LINE__);

    return -1;
}

uint32_t Kernel::virtualFileSystem::getFileSize(const char* filename) {
    Log(Error, "virtual filesystem failed %s:%d", __FILE__, __LINE__);
    return -1;
}
