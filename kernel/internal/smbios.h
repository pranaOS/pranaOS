#pragma once

#include "systemcomponent.h"
#include <system/console.h>
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
            common::uint8_t type;
            common::uint8_t length;
            common::uint16_t handle;
        } __attribute__((packed));

        struct smbiosBiosInfo : public smbiosTag {
            common::uint8_t vendor;
            common::uint8_t version;
            common::uint16_t startSegment;
            common::uint8_t releaseDate;
            common::uint8_t romSize;
            common::uint64_t characteristics;
            common::uint8_t extensionChar1;
            common::uint8_t extensionChar2;
            common::uint8_t biosMajorRelease;
            common::uint8_t biosMinorRelease;
            common::uint8_t controllerMajorRelease;
            common::uint8_t controllerMinorRelease;
        } __attribute__((packed));

        struct smbiosSystemInfo : public smbiosTag {
            common::uint8_t manufacturer;
            common::uint8_t productName;
            common::uint8_t version;
            common::uint8_t serialNumber;

            struct {
                common::uint32_t time_low;
                common::uint16_t time_mid;
                common::uint16_t timeHiAndVersion;
                common::uint8_t clockSeqHiAndReserved;
                common::uint8_t clockSeqLow;
                common::uint8_t node[6];
            } uuid;

            common::uint8_t wakeupType;

            common::uint8_t sku;
            common::uint8_t family;
        } __attribute__((packed));

        struct smbiosBaseBoardInformation : public smbiosTag {
            common::uint8_t manufacturer;
            common::uint8_t product;
            common::uint8_t version;
            common::uint8_t serial;
            common::uint8_t assertTag;
            common::uint8_t features;
            common::uint8_t locationInChassis;
            common::uint16_t chassisHandle;
            common::uint8_t boardType;
            common::uint8_t numObjHandles;
            common::uint16_t objectHandles[1]; 
        } __attribute__((packed));

        struct smbiosSystemEnclosureInformation : public smbiosTag {
            common::uint8_t manufacturer;
            common::uint8_t type;
            common::uint8_t version;
            common::uint8_t serialNumber;
            common::uint8_t assetTag;
            common::uint8_t bootupState;
            common::uint8_t psuState;
            common::uint8_t thermalState;
            common::uint8_t securityStatus;
            common::uint8_t oemSpecific[4];
            common::uint8_t height;
            common::uint8_t numOfPowerCords;
            common::uint8_t numOfElements;
            common::uint8_t elementRecordLength;

            struct element {
                common::uint8_t type;
                common::uint8_t minimum;
                common::uint8_t maximum;
            } elements[1];
        } __attribute__((packed));

        struct smbiosProcessorInformation : public smbiosTag {
            common::uint8_t socketDesignation;
            common::uint8_t processorType;
            common::uint8_t processorFamily;
            common::uint8_t manufacturer;
            common::uint64_t processorID;
            common::uint8_t version;
            common::uint8_t voltage;
            common::uint16_t clock;
            common::uint16_t maxSpeed;
            common::uint16_t currentSpeed;
            common::uint8_t status;
            common::uint8_t upgrade;
            common::uint16_t L1;
            common::uint16_t L2;
            common::uint16_t L3;

            common::uint8_t serialNumber;
            common::uint8_t assertTag;
            common::uint8_t partNumber;

            common::uint8_t totalCores;
            common::uint8_t activeCores;
            common::uint8_t threads;
            common::uint16_t characteristics;
        } __attribute__((packed));

        struct smbiosCacheInformation : public smbiosTag {
            common::uint8_t socketDesignation;
            common::uint16_t cacheConfiguration;
            common::uint16_t maximumCacheSize;
            common::uint16_t installedSize;
            common::uint16_t supportedSRAMType;
            common::uint16_t currentSRAMType;

            common::uint8_t cacheSpeed;
            common::uint8_t errorCorrectionType;
            common::uint8_t systemCacheType;
            common::uint8_t associativity;

            common::uint32_t maximumCacheSize2;
            common::uint32_t installedSize2;
        } __attribute__((packed));

        struct smbiosSystemSlotInformation : public smbiosTag {
            common::uint8_t slotDesignation;
            common::uint8_t slotType;
            common::uint8_t slotDataBusWidth;
            common::uint8_t currentUsage;
            common::uint8_t slotLength;
            common::uint16_t slotID;
            common::uint8_t slotCharacteristics1;

            common::uint8_t slotCharacteristics2;

            common::uint16_t segmentGroupNumber;
            common::uint8_t busNumber;
            common::uint8_t deviceFunctionNumber;

            common::uint8_t dataBusWidth;
        } __attribute__((packed));

        struct smbiosPhysicalMemoryArray : public smbiosTag {
            common::uint8_t location;
            common::uint8_t use;
            common::uint8_t memoryErrorCorrection;
            common::uint32_t maximumCapacity;
            common::uint16_t memoryErrorInformationHandle;
            common::uint16_t numberOfMemoryDevices;

            common::uint64_t extendedMaximumCapacity;
        } __attribute__((packed));

        struct smbiosMemoryDevice : public smbiosTag {
            common::uint16_t memoryArrayHandle;
            common::uint16_t memoryErrorInformationHandle;
            common::uint16_t totalWidth;
            common::uint16_t dataWidth;
            common::uint16_t size;
            common::uint8_t formFactor;
            common::uint8_t deviceSet;
            common::uint8_t deviceLocator;
            common::uint8_t bankLocator;
            common::uint8_t memoryType;
            common::uint16_t typeDetail;
            common::uint16_t speed;
            common::uint8_t manufacturer;
            common::uint8_t serialNumber;
            common::uint8_t assetTag;
            common::uint8_t partNumber;
            common::uint8_t attributes;
            common::uint32_t extendedSize;
            common::uint16_t configuredMemorySpeed;
            common::uint16_t minimumVoltage;
            common::uint16_t maximumVoltage;
            common::uint16_t configuredVoltage;
            common::uint8_t memoryTechnology;
            common::uint16_t memoryOperatingModeCapability;
            common::uint8_t firwareVersion;
            common::uint16_t moduleManufacturerID;
            common::uint16_t moduleProductID;
            common::uint16_t memorySubsystemControllerManufacturerID;
            common::uint16_t memorySubsystemControllerProductID;
            common::uint64_t nonVolatileSize;
            common::uint64_t volatileSize;
            common::uint64_t cacheSize;
            common::uint64_t logicalSize;
        } __attribute__((packed));

        struct smbiosBootInformation : public smbiosTag {
            common::uint8_t reserved[6];
            common::uint8_t bootStatus;
        } __attribute__((packed));

        struct smbiosEntryPoint {
            char entryPointString[4];               
            common::uint8_t checksum;               
            common::uint8_t length;                 
            common::uint8_t majorVersion;           
            common::uint8_t minorVersion;           
            common::uint16_t maxStructureSize;      
            common::uint8_t entryPointRevision;     
            char formattedArea[5];                  
            char entryPointString2[5];              
            common::uint8_t checksum2;              
            common::uint16_t tableLength;           
            common::uint32_t tableAddress;	        
            common::uint16_t numberOfStructures;    
            common::uint8_t bcdRevision;            
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