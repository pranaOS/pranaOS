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
        uint8_t external : 1;
        uint8_t table : 2;
        uint16_t tableIndex : 13;
    } __attribute__((packed));
        

    class exceptions {
    public:
        static uint32_t handleException(uint32_t number, uint32_t esp);    
        static void enablePagefaultAutoFix();
        static void disablePagefaultAutoFix();
    
    private:
        static uint32_t divideByZero(uint32_t esp);
        static uint32_t generalProtectionFault(uint32_t esp);
        static uint32_t pageFault(uint32_t esp);
        static uint32_t trapException(uint32_t esp);
        static uint32_t floatingPointException(uint32_t esp);
        static uint32_t stackSegmentFault(uint32_t esp);
        static void showStacktrace(uint32_t esp);
    };
}

/**
 * @brief exception init 
 * 
 */
void exception_init();