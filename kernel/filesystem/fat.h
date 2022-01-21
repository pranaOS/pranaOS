//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "virtualfilesystem.h

namespace Kernel {

    struct Fat32 {
        ak::uint8_t     bootCode[3];
        ak::uint8_t     oemId[8];
        ak::uint16_t    bytesPerSector;
        ak::uint8_t     sectorsPerCluster;
        ak::uint16_t    reservedSectors;
        ak::uint8_t     numOfFats;
        ak::uint16_t    numDirEntries;
        ak::uint16_t    totalSectorsSmall;
        ak::uint8_t     mediaDescriptorType;
        ak::uint16_t    sectorsPerFat12_16;
        ak::uint16_t    sectorsPerTrack;
        ak::uint16_t    numHeads;
        ak::uint32_t    hiddenSectors;
        ak::uint32_t    totalSectorsBig;
        ak::uint32_t    sectorsPerFat32;
        ak::uint16_t    flags;
        ak::uint16_t    fatVersionNum;
        ak::uint32_t    footDirCluster;
        ak::uint16_t    fsInfoSector;
        ak::uint16_t    backupBootSector;
        ak::uint8_t     reserved[12];
        ak::uint8_t     driveNum;
        ak::uint8_t     winNTFlags;
        ak::uint8_t     signature;
        ak::uint32_t    volumeIDSerial;
        ak::uint8_t     volumeLabel[11];
        ak::uint8_t     systemIDString[8];
        ak::uint8_t     bootCode[420];
        ak::uint16_t    bootSignature;
    } __attribute__((packed));

    struct Fat32Info {
        ak::uint32_t    signature1;
        ak::uint8_t     reserved1[480];
        ak::uint32_t    signature2;
        ak::uint32_t    lastFreeCluster;
        ak::uint32_t    startSearchCluster;
        ak::uint8_t     reserved2[12];
        ak::uint32_t    signature3;
    } __attribute__((packed));

    
    struct DirectoryEntry {
        ak::uint8_t     fileName[11];       
        ak::uint8_t     attributes;         
        ak::uint8_t     reserved;           
        ak::uint8_t     creationTimeTenth;  
        ak::uint16_t    creationTime;       
        ak::uint16_t    creationDate;       
        ak::uint16_t    accessDate;         
        ak::uint16_t    highFirstCluster;   
        ak::uint16_t    modifyTime;         
        ak::uint16_t    modifyDate;         
        ak::uint16_t    lowFirstCluster;    
        ak::uint32_t    fileSize;          
    } __attribute__((packed));

    struct lfnEntry {

    };

    struct fatEntryInfo {
        char* filename;
    };
    
}