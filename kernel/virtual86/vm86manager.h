/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <kernel/interrupthandler.h>
#include "vm86args.h"
#include <cpu/register.h>
#include <cpu/idt.h>
#include <cpu/tasksegment.h>

namespace Kernel {
    class virtual86Manager : public interruptHandler {
    public:
        virtual86Manager();

        /**
         * @brief handleInterrupt
         * 
         * @param esp 
         * @return uint32_t 
         */
        uint32_t handleInterrupt(uint32_t esp);

        /**
         * @brief callInterrupt
         * 
         * @param intNumber 
         * @param regs 
         */
        void callInterrupt(uint8_t intNumber, vm86Args* regs);

        /**
         * @brief executeCode
         * 
         * @param instructionStart 
         * @param args 
         */
        void executeCode(uint32_t instructionStart, uint32_t args);

    private:
        /**
         * @brief vm86Enter
         * 
         * @param ss 
         * @param sp 
         * @param cs 
         * @param ip 
         * @param arg 
         */
        void vm86Enter(uint16_t ss, uint16_t sp, uint16_t cs, uint16_t ip, uint32_t arg);

    };
}