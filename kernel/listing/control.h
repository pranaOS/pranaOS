/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <filesystem/virtualfilesystem.h>
#include <tasking/process.h>

namespace Kernel {
    class controller {
    protected:
        /**
         * @brief waiting queue 
         * 
         */
        ak::List<Thread*> waitingQueue;

        /**
         * @brief current required thread
         * 
         */
        Thread* currentReqThread;

        /**
         * @brief request busy is false
         * 
         */
        bool requestBusy = false;

    public:
        controller();

        virtual int beginListing(Thread *thread, ak::uint32_t arg1 = 0);
        virtual int getEntry(Thread* thread, int entry, ak::uint32_t bufPtr);
        virtual void endListing(Thread* thread);
    };
}