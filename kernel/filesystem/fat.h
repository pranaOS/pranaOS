//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "virtualfilesystem.h"

namespace Kernel {

    struct fat32 {
        uint8_t     bootCode[3];
        uint8_t     oemId[8];
        uint16_t    bytesPerSector;
        uint8_t     sectorsPerCluster;
        uint16_t    reservedSectors;
        uint8_t     numOfFats;
        uint16_t    numDirEntries;
        uint16_t    totalSectorsSmall;
        uint8_t     mediaDescriptorType;
        uint16_t    sectorsPerFat12_16;
        uint16_t    sectorsPerTrack;
        uint16_t    numHeads;
        uint32_t    hiddenSectors;
        uint32_t    totalSectorsBig;
        uint32_t    sectorsPerFat32;
        uint16_t    flags;
        uint16_t    fatVersionNum;
        uint32_t    rootDirCluster;
        uint16_t    fsInfoSector;
        uint16_t    backupBootSector;
        uint8_t     reserved[12];
        uint8_t     driveNum;
        uint8_t     winNTFlags;
        uint8_t     signature;
        uint32_t    volumeIDSerial;
        uint8_t     volumeLabel[11];
        uint8_t     systemIDString[8];
        uint8_t     bootCode[420];
        uint16_t    bootSignature;
    } __attribute__((packed));

    struct fat32Info {
        uint32_t    signature1;
        uint8_t     reserved1[480];
        uint32_t    signature2;
        uint32_t    lastFreeCluster;
        uint32_t    startSearchCluster;
        uint8_t     reserved2[12];
        uint32_t    signature3;
    } __attribute__((packed));

    struct directoryEntry {
        uint8_t     fileName[11];       
        uint8_t     attributes;         
        uint8_t     reserved;           
        uint8_t     creationTimeTenth;  
        uint16_t    creationTime;       
        uint16_t    creationDate;       
        uint16_t    accessDate;         
        uint16_t    highFirstCluster;   
        uint16_t    modifyTime;         
        uint16_t    modifyDate;         
        uint16_t    lowFirstCluster;    
        uint32_t    fileSize;           
    } __attribute__((packed));

    struct lfnEntry {
        uint8_t entryIndex;             
        uint8_t namePart1[10];         
        uint8_t Attributes;            
        uint8_t reserved_1;            
        uint8_t checksum;              
        uint8_t namePart2[12];         
        uint16_t reserved_2;           
        uint8_t namePart3[4];          
    } __attribute__((packed));

    struct fatEntryInfo {
        directoryEntry entry;                   
        char* filename;                         
        uint32_t sector;               
        uint32_t offsetInSector;       
    } __attribute__((packed));
    
    #define CLUSTER_END_32  0x0FFFFFF8
    #define CLUSTER_BAD_32  0x0FFFFFF7
    #define CLUSTER_FREE_32 0x00000000

    #define CLUSTER_END_16  0xFFF8
    #define CLUSTER_BAD_16  0xFFF7
    #define CLUSTER_FREE_16 0x0000
        
    #define CLUSTER_END_12  0xFF8
    #define CLUSTER_BAD_12  0xFF7
    #define CLUSTER_FREE_12 0x000

    #define CLUSTER_END     (this->FatType == FAT12 ? CLUSTER_END_12 : (this->FatType == FAT16 ? CLUSTER_END_16 : CLUSTER_END_32))
    #define CLUSTER_FREE    (this->FatType == FAT12 ? CLUSTER_FREE_12 : (this->FatType == FAT16 ? CLUSTER_FREE_16 : CLUSTER_FREE_32))
    #define CLUSTER_BAD     (this->FatType == FAT12 ? CLUSTER_BAD_12 : (this->FatType == FAT16 ? CLUSTER_BAD_16 : CLUSTER_BAD_32))

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

	#define GET_CLUSTER(e) (e.LowFirstCluster | (e.HighFirstCluster << (16)))

    enum fatType {
        FAT12,
        FAT16,
        FAT32
    };
        
    class fat : public VirtualFileSystem {
    private: 
        fatType fattpye;                    
        char* FatTypeString = 0;            

        uint16_t bytesPerSector = 0;        
        uint32_t rootDirSectors = 0;        
        uint8_t sectorsPerCluster = 0;     
        uint32_t clusterSize = 0;          

        uint32_t firstDataSector = 0;      
        uint32_t firstFatSector = 0;       
        uint32_t rootDirCluster = 0;       
        uint32_t totalClusters = 0;        

        uint8_t* readBuffer = 0;            
        fat32Info fsInfo;                

    private:
        uint32_t clusterToSector(uint32_t cluster);
        uint32_t readTable(uint32_t cluster);
        void writeTable(uint32_t cluster, uint32_t value);
        uint32_t allocateCluster();

        void clearCluster(uint32_t cluster);
        char* parseLFNEntries(List<lfnEntry>* entries, directoryEntry sfnEntry);
        char* parseShortFilename(char* fatName);

        uint8_t checksum(char* filename);

        List<fatEntryInfo> getDirectoryEntries(uint32_t dirCluster, bool rootDirectory = false);

        fatEntryInfo* seachInDirectory(char* name, uint32_t dirCluster, bool rootDirectory = false);

        fatEntryInfo* getEntryByPath(char* path);

        List<lfnEntry> createLFNEntriesFromName(char* name, int num, uint8_t checksum);

        char* createShortFilename(char* name);

        bool writeLongFilenameEntries(List<LFNEntry>* entries, uint32_t targetCluster, uint32_t targetSector, uint32_t sectorOffset, bool rootDirectory);

        bool writeDirectoryEntry(directoryEntry entry, uint32_t targetSector, uint32_t sectorOffset, bool rootDirectory);

        bool findEntryStartpoint(uint32_t cluster, uint32_t entryCount, bool rootdir, uint32_t* targetCluster, uint32_t* targetSector, uint32_t* sectorOffset);

        directoryEntry* createEntry(uint32_t parentCluster, char* name, uint8_t attr, bool rootdir, uint32_t targetCluster, uint32_t* sectorPlaced);

        int createNewDirFileEntry(const char* path, uint8_t attributes);

        bool modifyEntry(fatEntryInfo* entry, directoryEntry newVersion);

        uint16_t fatTime();

        uint16_t fatDate();

    public:
        fat(Disk* disk, uint32_t start, uint32_t size);
        ~fat();

        bool initialize();

        int readFile(const char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
        int writeFile(const char* filename, uint8_t* buffer, uint32_t len, bool create = true);

        bool fileExists(const char* filename);
        bool directoryExists(const char* filename);

        int createFile(const char* path);
        int createDirectory(const char* path);

        uint32_t getFileSize(const char* filename);
        List<LibC::vfsEntry>* directoryList(const char* path);
    };
}