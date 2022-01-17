#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <system/console.h>
#include "idt.h"

namespace Kernel {
    struct selectorErrorCode {
        ak::uint8_t external : 1;
        ak::uint8_t table : 2;
        ak::uint16_t tableIndex : 13;
    } __attribute__((packed));

    class exceptions {
    public:
        static ak::uint32_t divideByZero(ak::uint32_t esp);
        static ak::uint32_t generalProtectionFault(ak::uint32_t esp);

    private:
    };
}