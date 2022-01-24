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