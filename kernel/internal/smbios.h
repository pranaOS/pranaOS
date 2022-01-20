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
            ak::uint8_t type;
            ak::uint8_t length;
            ak::uint16_t handle;
        } __attribute__((packed));

        struct smbiosBiosInfo : public smbiosTag {
            ak::uint8_t vendor;
            ak::uint8_t version;
            ak::uint16_t startSegment;
            ak::uint8_t releaseDate;
            ak::uint8_t romSize;
            ak::uint64_t characteristics;
            ak::uint8_t extensionChar1;
            ak::uint8_t extensionChar2;
            ak::uint8_t biosMajorRelease;
            ak::uint8_t biosMinorRelease;
            ak::uint8_t controllerMajorRelease;
            ak::uint8_t controllerMinorRelease;
        } __attribute__((packed));

        struct smbiosSystemInfo : public smbiosTag {
            ak::uint8_t manufacturer;
            ak::uint8_t productName;
            ak::uint8_t version;
            ak::uint8_t serialNumber;

            struct {
                ak::uint32_t time_low;
                ak::uint16_t time_mid;
                ak::uint16_t timeHiAndVersion;
                ak::uint8_t clockSeqHiAndReserved;
                ak::uint8_t clockSeqLow;
                ak::uint8_t node[6];
            } uuid;

            ak::uint8_t wakeupType;

            ak::uint8_t sku;
            ak::uint8_t family;
        } __attribute__((packed));

        struct smbiosBaseBoardInformation : public smbiosTag {
            ak::uint8_t manufacturer;
            ak::uint8_t product;
            ak::uint8_t version;
            ak::uint8_t serial;
            ak::uint8_t assertTag;
            ak::uint8_t features;
            ak::uint8_t locationInChassis;
            ak::uint16_t chassisHandle;
            ak::uint8_t boardType;
            ak::uint8_t numObjHandles;
            ak::uint16_t objectHandles[1]; 
        } __attribute__((packed));

        struct smbiosSystemEnclosureInformation : public smbiosTag {
            ak::uint8_t manufacturer;
            ak::uint8_t type;
            ak::uint8_t version;
            ak::uint8_t serialNumber;
            ak::uint8_t assetTag;
            ak::uint8_t bootupState;
            ak::uint8_t psuState;
            ak::uint8_t thermalState;
            ak::uint8_t securityStatus;
            ak::uint8_t oemSpecific[4];
            ak::uint8_t height;
            ak::uint8_t numOfPowerCords;
            ak::uint8_t numOfElements;
            ak::uint8_t elementRecordLength;

            struct element {
                ak::uint8_t type;
                ak::uint8_t minimum;
                ak::uint8_t maximum;
            } elements[1];
        } __attribute__((packed));

        struct smbiosProcessorInformation : public smbiosTag {
            ak::uint8_t socketDesignation;
            ak::uint8_t processorType;
            ak::uint8_t processorFamily;
            ak::uint8_t manufacturer;
            ak::uint64_t processorID;
            ak::uint8_t version;
            ak::uint8_t voltage;
            ak::uint16_t clock;
            ak::uint16_t maxSpeed;
            ak::uint16_t currentSpeed;
            ak::uint8_t status;
            ak::uint8_t upgrade;
            ak::uint16_t L1;
            ak::uint16_t L2;
            ak::uint16_t L3;

            ak::uint8_t serialNumber;
            ak::uint8_t assertTag;
            ak::uint8_t partNumber;

            ak::uint8_t totalCores;
            ak::uint8_t activeCores;
            ak::uint8_t threads;
            ak::uint16_t characteristics;
        } __attribute__((packed));

        struct smbiosCacheInformation : public smbiosTag {
            ak::uint8_t socketDesignation;
            ak::uint16_t cacheConfiguration;
            ak::uint16_t maximumCacheSize;
            ak::uint16_t installedSize;
            ak::uint16_t supportedSRAMType;
            ak::uint16_t currentSRAMType;

            ak::uint8_t cacheSpeed;
            ak::uint8_t errorCorrectionType;
            ak::uint8_t systemCacheType;
            ak::uint8_t associativity;

            ak::uint32_t maximumCacheSize2;
            ak::uint32_t installedSize2;
        } __attribute__((packed));

        struct smbiosSystemSlotInformation : public smbiosTag {
            ak::uint8_t slotDesignation;
            ak::uint8_t slotType;
            ak::uint8_t slotDataBusWidth;
            ak::uint8_t currentUsage;
            ak::uint8_t slotLength;
            ak::uint16_t slotID;
            ak::uint8_t slotCharacteristics1;

            ak::uint8_t slotCharacteristics2;

            ak::uint16_t segmentGroupNumber;
            ak::uint8_t busNumber;
            ak::uint8_t deviceFunctionNumber;

            ak::uint8_t dataBusWidth;
        } __attribute__((packed));

        struct smbiosPhysicalMemoryArray : public smbiosTag {
            ak::uint8_t location;
            ak::uint8_t use;
            ak::uint8_t memoryErrorCorrection;
            ak::uint32_t maximumCapacity;
            ak::uint16_t memoryErrorInformationHandle;
            ak::uint16_t numberOfMemoryDevices;

            ak::uint64_t extendedMaximumCapacity;
        } __attribute__((packed));

        struct smbiosMemoryDevice : public smbiosTag {
            ak::uint16_t memoryArrayHandle;
            ak::uint16_t memoryErrorInformationHandle;
            ak::uint16_t totalWidth;
            ak::uint16_t dataWidth;
            ak::uint16_t size;
            ak::uint8_t formFactor;
            ak::uint8_t deviceSet;
            ak::uint8_t deviceLocator;
            ak::uint8_t bankLocator;
            ak::uint8_t memoryType;
            ak::uint16_t typeDetail;
            ak::uint16_t speed;
            ak::uint8_t manufacturer;
            ak::uint8_t serialNumber;
            ak::uint8_t assetTag;
            ak::uint8_t partNumber;
            ak::uint8_t attributes;
            ak::uint32_t extendedSize;
            ak::uint16_t configuredMemorySpeed;
            ak::uint16_t minimumVoltage;
            ak::uint16_t maximumVoltage;
            ak::uint16_t configuredVoltage;
            ak::uint8_t memoryTechnology;
            ak::uint16_t memoryOperatingModeCapability;
            ak::uint8_t firwareVersion;
            ak::uint16_t moduleManufacturerID;
            ak::uint16_t moduleProductID;
            ak::uint16_t memorySubsystemControllerManufacturerID;
            ak::uint16_t memorySubsystemControllerProductID;
            ak::uint64_t nonVolatileSize;
            ak::uint64_t volatileSize;
            ak::uint64_t cacheSize;
            ak::uint64_t logicalSize;
        } __attribute__((packed));

        struct smbiosBootInformation : public smbiosTag {
            ak::uint8_t reserved[6];
            ak::uint8_t bootStatus;
        } __attribute__((packed));

        struct smbiosEntryPoint {
            char entryPointString[4];               
            ak::uint8_t checksum;               
            ak::uint8_t length;                 
            ak::uint8_t majorVersion;           
            ak::uint8_t minorVersion;           
            ak::uint16_t maxStructureSize;      
            ak::uint8_t entryPointRevision;     
            char formattedArea[5];                  
            char entryPointString2[5];              
            ak::uint8_t checksum2;              
            ak::uint16_t tableLength;           
            ak::uint32_t tableAddress;	        
            ak::uint16_t numberOfStructures;    
            ak::uint8_t bcdRevision;            
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