//
// Created by KrisnaPranav on 24/01/22.
//

#include <ak/string.h>
#include <ak/memoperator.h>
#include <kernel/kernel.h>
#include "vfsmanager.h"

using namespace Kernel;
using namespace ak;

vfsManager::vfsManager() {
    this->Filesystems = new List<virtualFileSystem*>();
}

int vfsManager::extractDiskNumber(const char* path, int* idSizeReturn) {
    if(String::contains(path, ':') && String::contains(path, PATH_SEPERATOR_C)) {
        int idLength = String::indexOf(path, ":");
        char* idStr = new char[idLength];
    }
}

void vfsManager::mount(virtualFileSystem* vfs) {
    this->Filesystems->push_back(vfs);
}

void vfsManager::unmount(virtualFileSystem* vfs) {
    this->Filesystems->remove(vfs);
}

void vfsManager::unmountByDisk(Disk* disk) {
    for (virtualFileSystem* vfs : *Filesystems)
        if (vfs->disk == disk)
            unmount(vfs);
}

bool vfsManager::searchBootPartition() {
    List<Disk*> possibleDisks;
    char* pathString = "####:\\disk\\pranaOS.bin";

}


uint32_t vfsManager::fileSize(const char* filename) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(filename, &idSize);
    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->GetFileSize(filename + idSize + 2);
    else
        return -1;
}

int vfsManager::readFile(const char* path, uint8_t* buffer, uint32_t offset, uint32_t len) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->ReadFile(path + idSize + 2, buffer, offset, len);
    else
        return -1;
}

int vfsManager::writeFile(const char* path, uint8_t* buffer, uint32_t len, bool create) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->WriteFile(path + idSize + 2, buffer, len, create);
    else
        return -1;
}


bool vfsManager::directoryExists(const char* path) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2)
            return true;
        else
            return Filesystems->GetAt(disk)->DirectoryExists(path + idSize + 2);
    }
    else
        return false;
}

bool vfsManager::fileExists(const char* path) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->FileExists(path + idSize + 2);
    else
        return false;
}

int vfsManager::createFile(const char* path) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) //Only disk part, like 0:\ ofcourse this is a directory as well
            return true;
        else
            return Filesystems->GetAt(disk)->CreateFile(path + idSize + 2);
    }
    else
        return -1;
}

int vfsManager::createDirectory(const char* path) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2)
            return true;
        else
            return Filesystems->GetAt(disk)->CreateDirectory(path + idSize + 2);
    }
    else
        return -1;
}

bool vfsManager::ejectDrive(const char* path) {
    uint8_t idSize = 0;
    int disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        VirtualFileSystem* fs = Filesystems->GetAt(disk);
        return fs->disk->controller->EjectDrive(fs->disk->controllerIndex);
    }
    else
        return false;
}