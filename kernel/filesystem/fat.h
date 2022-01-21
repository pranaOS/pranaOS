//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "virtualfilesystem.h

namespace Kernel {

    struct fat32 {
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

    struct fat32Info {
        ak::uint32_t    signature1;
        ak::uint8_t     reserved1[480];
        ak::uint32_t    signature2;
        ak::uint32_t    lastFreeCluster;
        ak::uint32_t    startSearchCluster;
        ak::uint8_t     reserved2[12];
        ak::uint32_t    signature3;
    } __attribute__((packed));

    
    struct directoryEntry {
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
        ak::uint8_t entryIndex;             
        ak::uint8_t namePart1[10];          
        ak::uint8_t Attributes;             
        ak::uint8_t reserved_1;             
        ak::uint8_t checksum;               
        ak::uint8_t namePart2[12];          
        ak::uint16_t reserved_2;            
        ak::uint8_t namePart3[4];           
    } __attribute__((packed));


    struct fatEntryInfo {
        directoryEntry entry;                   
        char* filename;                         
        ak::uint32_t sector;                
        ak::uint32_t offsetInSector;        
    } __attribute__((packed));

    enum fatType {
        FAT12,
        FAT16,
        FAT32,
    };

    #define CLUSTER_END_32  0x0FFFFFF8
    #define CLUSTER_BAD_32  0x0FFFFFF7
    #define CLUSTER_FREE_32 0x00000000

    #define CLUSTER_END_16  0xFFF8
    #define CLUSTER_BAD_16  0xFFF7
    #define CLUSTER_FREE_16 0x0000

    #define CLUSTER_END_12  0xFF8
    #define CLUSTER_BAD_12  0xFF7
    #define CLUSTER_FREE_12 0x000

    #define CLUSTER_END     (this->fatType == FAT12 ? CLUSTER_END_12 : (this->fatType == FAT16 ? CLUSTER_END_16 : CLUSTER_END_32))
    #define CLUSTER_FREE    (this->fatType == FAT12 ? CLUSTER_FREE_12 : (this->fatType == FAT16 ? CLUSTER_FREE_16 : CLUSTER_FREE_32))
    #define CLUSTER_BAD     (this->fatType == FAT12 ? CLUSTER_BAD_12 : (this->fatType == FAT16 ? CLUSTER_BAD_16 : CLUSTER_BAD_32))

    #define ATTR_READ_ONLY  0x01
    #define ATTR_HIDDEN 	0x02
    #define ATTR_SYSTEM     0x04
    #define ATTR_VOLUME_ID  0x08
    #define ATTR_DIRECTORY	0x10
    #define ATTR_ARCHIVE    0x20
    #define ATTR_LONG_NAME 	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

    #define ENTRY_END       0x00
    #define ENTRY_UNUSED    0xE5
    #define LFN_ENTRY_END   0x40

}