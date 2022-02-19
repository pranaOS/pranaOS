/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>
#include <ak/list.h>
#include <cpu/register.h>

namespace Kernel {
    typedef struct {
        char* name;
        ak::uint32_t address;
        char type;
    } genericsymbol_t;

    typedef struct stackframe {
        struct stackframe* next;
        ak::uint32_t addr;
    } stackframe_t;

    class symbolDebugger {
    public:
        /**
         * @brief Construct a new symbol Debugger object
         * 
         * @param symFile 
         * @param kernel 
         */
        symbolDebugger(char* symFile, bool kernel = false);

        /**
         * @brief stack trace
         * 
         * @param esp 
         */
        void stacktrace(CPUState* esp);

        /**
         * @brief update symboldebugger
         * 
         */
        void update();
        
        /**
         * @brief send update to a host
         * 
         */
        void sendUpdateHost();

        /**
         * @brief print memory dump
         * 
         * @param address 
         * @param size 
         * @param virtMemory 
         */
        void printMemoryDump(ak::uint32_t address, ak::uint32_t size, bool virtMemory);


    private:
        ak::List<genericsymbol_t> symbolTable;
        char messageBuffer[200] = {0};
        ak::uint32_t pageAccessAddress = 0;

        bool isKernel = false;
        int serialIndex = 0;

        /**
         * @brief find symbol
         * 
         * @param address 
         * @param offset 
         * @return const char* 
         */
        const char* findSymbol(ak::uint32_t address, ak::uint32_t* offset);

        /**
         * @brief handle debug command
         * 
         * @param size 
         */
        void handleDebugCommand(int size);
        
        /**
         * @brief print page item
         * 
         * @param item 
         * @param table 
         * @param pdIndex 
         * @param ptIndex 
         */
        void printPageItem(void* item, bool table, ak::uint16_t pdIndex, ak::uint16_t ptIndex);
    };
}