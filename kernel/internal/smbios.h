#pragma once

#include "systemcomponent.h"
#include <kernel/console.h>
#include <ak/string.h>
#include <ak/list.h>

namespace Kernel {
    namespace system {
        enum smbiosTableType  {
            BIOSInformation = 0,
            SystemInformation = 1,
            BaseBoardInformation = 2,
            EnclosureInformation = 3,
            ProcessorInformation = 4,
            MemoryControllerInformation = 5,
            MemoryModuleInformation = 6,
            CacheInformation = 7,
            PortConnectorInformation = 8,
            SystemSlotsInformation = 9,
            OnBoardDevicesInformation = 10,
            OEMStrings = 11,
            SystemConfigurationOptions = 12,
            BIOSLanguageInformation = 13,
            GroupAssociations = 14,
            SystemEventLog = 15,
            PhysicalMemoryArray = 16,
            MemoryDevice = 17,
            MemoryErrorInformation = 18,
            MemoryArrayMappedAddress = 19,
            MemoryDeviceMappedAddress = 20,
            SystemBootInformation = 32,
            EndOfTable = 127
        };

        struct smbiosTag {
            uint8_t type;
            uint8_t length;
            uint16_t handle;
        } __attribute__((packed));

        struct smbiosBiosInfo : public smbiosTag {
            uint8_t vendor;
            uint8_t version;
            uint16_t startSegment;
            uint8_t releaseDate;
            uint8_t romSize;
            uint64_t characteristics;
            uint8_t extensionChar1;
            uint8_t extensionChar2;
            uint8_t biosMajorRelease;
            uint8_t biosMinorRelease;
            uint8_t controllerMajorRelease;
            uint8_t controllerMinorRelease;
        } __attribute__((packed));

        struct smbiosSystemInfo : public smbiosTag {
            uint8_t manufacturer;
            uint8_t productName;
            uint8_t version;
            uint8_t serialNumber;

            struct {
                uint32_t time_low;
                uint16_t time_mid;
                uint16_t timeHiAndVersion;
                uint8_t clockSeqHiAndReserved;
                uint8_t clockSeqLow;
                uint8_t node[6];
            } uuid;

            uint8_t wakeupType;

            uint8_t sku;
            uint8_t family;
        } __attribute__((packed));

        struct smbiosBaseBoardInformation : public smbiosTag {
            uint8_t manufacturer;
            uint8_t product;
            uint8_t version;
            uint8_t serial;
            uint8_t assertTag;
            uint8_t features;
            uint8_t locationInChassis;
            uint16_t chassisHandle;
            uint8_t boardType;
            uint8_t numObjHandles;
            uint16_t objectHandles[1]; 
        } __attribute__((packed));

        struct smbiosSystemEnclosureInformation : public smbiosTag {
            uint8_t manufacturer;
            uint8_t type;
            uint8_t version;
            uint8_t serialNumber;
            uint8_t assetTag;
            uint8_t bootupState;
            uint8_t psuState;
            uint8_t thermalState;
            uint8_t securityStatus;
            uint8_t oemSpecific[4];
            uint8_t height;
            uint8_t numOfPowerCords;
            uint8_t numOfElements;
            uint8_t elementRecordLength;

            struct element {
                uint8_t type;
                uint8_t minimum;
                uint8_t maximum;
            } elements[1];
        } __attribute__((packed));

        struct smbiosProcessorInformation : public smbiosTag {
            uint8_t socketDesignation;
            uint8_t processorType;
            uint8_t processorFamily;
            uint8_t manufacturer;
            uint64_t processorID;
            uint8_t version;
            uint8_t voltage;
            uint16_t clock;
            uint16_t maxSpeed;
            uint16_t currentSpeed;
            uint8_t status;
            uint8_t upgrade;
            uint16_t L1;
            uint16_t L2;
            uint16_t L3;

            uint8_t serialNumber;
            uint8_t assertTag;
            uint8_t partNumber;

            uint8_t totalCores;
            uint8_t activeCores;
            uint8_t threads;
            uint16_t characteristics;
        } __attribute__((packed));

        struct smbiosCacheInformation : public smbiosTag {
            uint8_t socketDesignation;
            uint16_t cacheConfiguration;
            uint16_t maximumCacheSize;
            uint16_t installedSize;
            uint16_t supportedSRAMType;
            uint16_t currentSRAMType;

            uint8_t cacheSpeed;
            uint8_t errorCorrectionType;
            uint8_t systemCacheType;
            uint8_t associativity;

            uint32_t maximumCacheSize2;
            uint32_t installedSize2;
        } __attribute__((packed));

        struct smbiosSystemSlotInformation : public smbiosTag {
            uint8_t slotDesignation;
            uint8_t slotType;
            uint8_t slotDataBusWidth;
            uint8_t currentUsage;
            uint8_t slotLength;
            uint16_t slotID;
            uint8_t slotCharacteristics1;

            uint8_t slotCharacteristics2;

            uint16_t segmentGroupNumber;
            uint8_t busNumber;
            uint8_t deviceFunctionNumber;

            uint8_t dataBusWidth;
        } __attribute__((packed));

        struct smbiosPhysicalMemoryArray : public smbiosTag {
            uint8_t location;
            uint8_t use;
            uint8_t memoryErrorCorrection;
            uint32_t maximumCapacity;
            uint16_t memoryErrorInformationHandle;
            uint16_t numberOfMemoryDevices;

            uint64_t extendedMaximumCapacity;
        } __attribute__((packed));

        struct smbiosMemoryDevice : public smbiosTag {
            uint16_t memoryArrayHandle;
            uint16_t memoryErrorInformationHandle;
            uint16_t totalWidth;
            uint16_t dataWidth;
            uint16_t size;
            uint8_t formFactor;
            uint8_t deviceSet;
            uint8_t deviceLocator;
            uint8_t bankLocator;
            uint8_t memoryType;
            uint16_t typeDetail;
            uint16_t speed;
            uint8_t manufacturer;
            uint8_t serialNumber;
            uint8_t assetTag;
            uint8_t partNumber;
            uint8_t attributes;
            uint32_t extendedSize;
            uint16_t configuredMemorySpeed;
            uint16_t minimumVoltage;
            uint16_t maximumVoltage;
            uint16_t configuredVoltage;
            uint8_t memoryTechnology;
            uint16_t memoryOperatingModeCapability;
            uint8_t firwareVersion;
            uint16_t moduleManufacturerID;
            uint16_t moduleProductID;
            uint16_t memorySubsystemControllerManufacturerID;
            uint16_t memorySubsystemControllerProductID;
            uint64_t nonVolatileSize;
            uint64_t volatileSize;
            uint64_t cacheSize;
            uint64_t logicalSize;
        } __attribute__((packed));

        struct smbiosBootInformation : public smbiosTag {
            uint8_t reserved[6];
            uint8_t bootStatus;
        } __attribute__((packed));

        struct smbiosEntryPoint {
            char entryPointString[4];               
            uint8_t checksum;               
            uint8_t length;                 
            uint8_t majorVersion;           
            uint8_t minorVersion;           
            uint16_t maxStructureSize;      
            uint8_t entryPointRevision;     
            char formattedArea[5];                  
            char entryPointString2[5];              
            uint8_t checksum2;              
            uint16_t tableLength;           
            uint32_t tableAddress;	        
            uint16_t numberOfStructures;    
            uint8_t bcdRevision;            
        } __attribute__((packed));

        class SMBIOS : public systemComponent {
        public:
            SMBIOS();
        
        private:
            void* tableAddress = 0;

            List<char*> extractStrings(SMBIOSTag* header);

            char* copyString(char* src);
        };
    }
}