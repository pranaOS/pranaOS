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
            ak::uint8_t type;
            ak::uint8_t length;
            ak::uint16_t handle;
        } __attribute__((packed));

        struct smbiosBiosInfo : public smbiosTag {
            ak::uint8_t vendor;
        }

        class SMBIOS : public systemComponent {
        private:
            void* tableAddress = 0;

            List<char*> extractStrings(smbiosTag* header);
            char* copyString(char* src);
        };
    }
}