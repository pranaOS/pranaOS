//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include <ak/types.h>
#include "virtualfilesystem.h"

namespace Kernel {
    struct directoryRecord {
        uint8_t length;
        uint8_t  earLength;
        uint8_t extentLocation;
        uint8_t extentLocationBe;
        uint8_t dataLength;
        uint8_t dataLengthBe;
        uint8_t datetime[7];
        uint8_t flags;
        uint8_t gapSize;
        uint8_t unitSize;
        uint8_t volSeqNumber;
        uint8_t volSeqNumberBe;
        uint8_t nameLength;
        char name[222];
    } __attribute__((packed));

    struct timeFormat {
        char year[4];
        char month[2];
        char day[2];
        char hour[2];
        char minute[2];
        char second[2];
        cahr hundrdSecond[2];
        int8_t timeZon;
    } __attribute__((packed));

    struct volumeDescriptor {
        uint8_t type;
        char identifier[5];
        uint8_t version;
        char data[2041];
    };

    #define ISODCL(from, to) (to - from + 1)

    struct primaryVolumeDescriptor {
        uint8_t type;
        char id                             [ISODCL (  2,     6)];
        uint8_t version;
        char reserved1                      [ISODCL (  8,     8)];
        char system_id                      [ISODCL (  9,    40)];
        char volume_id                      [ISODCL ( 41,    72)];
        char reserved2                      [ISODCL ( 73,    80)];
        uint64_t volumeSpaceSize;
        char reserved3                      [ISODCL ( 89,   120)];
        uint32_t volumeSetSize;
        uint32_t volumeSequenceNumber;
        uint32_t logicalBlockSize;
        uint64_t pathTableSize;
        uint32_t type1PathTable;
        uint32_t optType1PathTable;
        uint32_t typeMPathTable;
        uint32_t opt_type_m_path_table;
        directoryRecord rootDirectoryRecord;
        char volumeSetId                  [ISODCL (191,   318)];
        char publisherId                   [ISODCL (319,   446)];
        char preparerId                    [ISODCL (447,   574)];
        char applicationId                 [ISODCL (575,   702)];
        char copyrightFileId              [ISODCL (703,   739)];
        char abstractFileId               [ISODCL (740,   776)];
        char bibliographicFileId          [ISODCL (777,   813)];
        timeFormat creationDate         ;
        timeFormat modificationDate     ;
        timeFormat expirationDate       ;
        timeFormat effectiveDate        ;
        char fileStructureVersion         [ISODCL (882,   882)];
        char reserved4                      [ISODCL (883,   883)];
        uint8_t applicationData    [ISODCL (884,  1395)];
        char reserved5                      [ISODCL (1396,  2048)];
    } __attribute__((packed));

    #define ISO_START_SECTOR 0x10
    #define CDROM_SECTOR_SIZE 2048

    enum volumeDescriptorType {
        bootRecord = 0,
        pvDescriptor = 1,
        supplementaryVolumeDescriptor = 2,
        volumePartitionDescriptor = 3,
        volumeDescriptorSetTerminator = 255
    };

    enum isoEntryType {
        isoFile = 0,
        isoDirectory = 1
    };

    class isoFS : public virtualFileSystem {
      public:
        iso(Disk* disk, uint32_t start, uint32_t size);
        int readFile(const char* filename, uint8_t* buffer, uint32_t offset = 0, uint32_t len = -1);
        int writeFile(const char* filename, uint8_t* buffer, uint32_t len, bool create = true);

        bool fileExists(const char* filename);
        bool directoryExists(const char* filename);

        int createFile(const char* path);
        int createDirectory(const char* path);

        uint32_t getFileSize(const char* filename);
        List<LibC::vfsEntry>* directoryList(const char* path);

        bool initialize();
      private:
        directoryRecord* rootDirectory;
        directoryRecord* searchInDirectory(directoryRecord* searchIn, const char* name);
        directoryRecord* getEntry(const char* path);
        isoEntryType getEntryType(directoryRecord* entry);

        char* getRecordName(directoryRecord* record);
    };
}
