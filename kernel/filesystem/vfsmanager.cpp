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