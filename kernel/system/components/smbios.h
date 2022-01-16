#pragma once

#include "systemcomponent.h"
#include <system/console.h>
#include <ak/string.h>
#include <ak/list.h>

namespace Kernel {
    namespace system {
        enum smbiosTableType {
            BIOSInformation = 0,
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