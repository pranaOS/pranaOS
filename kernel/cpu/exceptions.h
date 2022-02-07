/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <kernel/console.h>
#include <stdarg.h>
#include <stdint.h>
#include "idt.h"

namespace Kernel {

    struct selectorErrorCode {
        ak::uint8_t external : 1;
        ak::uint8_t table : 2;
        ak::uint16_t tableIndex : 13;
    } __attribute__((packed));
        

    class exceptions {
    public:
        static ak::uint32_t handleException(ak::uint32_t number, ak::uint32_t esp);    
        static void enablePagefaultAutoFix();
        static void disablePagefaultAutoFix();
    
    private:
        static ak::uint32_t divideByZero(ak::uint32_t esp);
        static ak::uint32_t generalProtectionFault(ak::uint32_t esp);
        static ak::uint32_t pageFault(ak::uint32_t esp);
        static ak::uint32_t trapException(ak::uint32_t esp);
        static ak::uint32_t floatingPointException(ak::uint32_t esp);
        static ak::uint32_t stackSegmentFault(ak::uint32_t esp);
        static void showStacktrace(ak::uint32_t esp);
    };
}

/**
 * @brief exception init 
 * 
 */
void exception_init();