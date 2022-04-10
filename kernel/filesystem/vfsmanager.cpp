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
    this->Filesystems = new ak::List<virtualFileSystem*>();
}

int vfsManager::extractDiskNumber(const char* path, uint8_t* idSizeReturn) {
    if(String::contains(path, ':') && String::contains(path, PATH_SEPERATOR_C))
    {
        int idLength = String::indexOf(path, ':');

        char* idStr = new char[idLength];
        memOperator::memcpy(idStr, path, idLength);

        int idValue = 0;

        if(isalpha(idStr[0])) {
            switch (idStr[0]) {
                case 'b':
                case 'B': 
                    idValue = this->bootPartitionID;
                    break;      
                default:
                    delete idStr;
                    return -1;
                    break;
            }
        }

        else
            idValue = Convert::stringToInt(idStr);

        delete idStr;

        if(idSizeReturn != 0)
            *idSizeReturn = idLength;   
           
        return idValue;  
    }
    return -1;
}

void vfsManager::mount(virtualFileSystem* vfs) {
    this->Filesystems->push_back(vfs); 
}

void vfsManager::unmount(virtualFileSystem* vfs) {
    this->Filesystems->remove(vfs);
}

void vfsManager::unmountByDisk(Disk* disk) {
    for(virtualFileSystem* vfs : *Filesystems)
        if(vfs->disk == disk)
            unmount(vfs);
}

bool vfsManager::searchBootPartition() {
    List<Disk*> posibleDisks;
    char* pathString = "####:\\disks\\pranaOS.bin";

    uint8_t bootDevice = (system::mbi->boot_device & 0xFF000000) >> 24;
    biosDriveParameters* diskInfo = system::diskManager->getDriveInfoBios(bootDevice);
    if(diskInfo->bufLen > 0 && diskInfo->interfaceName[0] != '\0')  {
        DiskType bootDiskType = HardDisk;
        if(diskInfo->bytesPerSector == 2048) 
            bootDiskType = CDROM;
        if(String::strncmp("USB", diskInfo->interfaceName, 3)) 
            bootDiskType = USBDisk;
        
        for(Disk* disk : System::diskManager->allDisks)
            if(disk->type == bootDiskType)  {
                if(bootDiskType == HardDisk) { 
                    if(divide64(disk->size, diskInfo->bytesPerSector) == diskInfo->totalSectors)
                        posibleDisks.push_back(disk);
                }
                else
                    posibleDisks.push_back(disk);
            }
    }
    else { 
        for(Disk* disk : system::diskManager->allDisks)
            posibleDisks.push_back(disk);
    }

    for(int i = 0; i < Filesystems->size(); i++) {
        if(posibleDisks.indexOf(Filesystems->getAt(i)->disk) == -1)
            continue; 
        
        char* idStr = Convert::intToString(i);
        int idStrLen = String::strlen(idStr);

        memOperator::memcpy(pathString + (4-idStrLen), idStr, idStrLen);

        if(Filesystems->getAt(i)->fileExists(pathString + (4-idStrLen) + 3)) {
            this->bootPartitionID = i;
            return true;
        }
    }
    return false;
}

List<LibC::vfsEntry>* vfsManager::directoryList(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);
    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->getAt(disk)->directoryList(path + idSize + 2);
    else
        return 0;
}

uint32_t vfsManager::getFileSize(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);
    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->getAt(disk)->getFileSize(path + idSize + 2); 
    else
        return -1;
}

int vfsManager::readFile(const char* path, uint8_t* buffer, uint32_t offset, uint32_t len) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->getAt(disk)->readFile(path + idSize + 2, buffer, offset, len);
    else
        return -1;
}

int vfsManager::writeFile(const char* path, uint8_t* buffer, uint32_t len, bool create) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->getAt(disk)->writeFile(path + idSize + 2, buffer, len, create);
    else
        return -1;
}

bool vfsManager::fileExists(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->getAt(disk)->fileExists(path + idSize + 2);
    else
        return false;
}

bool vfsManager::DirectoryExists(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) 
            return true;
        else
            return Filesystems->getAt(disk)->directoryExists(path + idSize + 2);
    }
    else
        return false;
}

int vfsManager::createFile(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) 
            return true;
        else
            return Filesystems->getAt(disk)->createFile(path + idSize + 2);
    }
    else
        return -1;
}

int vfsManager::createDirectory(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) 
            return true;
        else
            return Filesystems->getAt(disk)->createDirectory(path + idSize + 2);
    }
    else
        return -1;
}

bool vfsManager::ejectDrive(const char* path) {
    uint8_t idSize = 0;
    int disk = extractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        VirtualFileSystem* fs = Filesystems->getAt(disk);
        return fs->disk->controller->EjectDrive(fs->disk->controllerIndex);
    }
    else
        return false;
}