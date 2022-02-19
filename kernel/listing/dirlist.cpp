/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <listing/dirlist.h>

using namespace Kernel;
using namespace LibC;

ak::List<vfsEntry>* currentDirList = 0;

/**
 * @brief Construct a new dir List::dir List object
 * 
 */
dirList::dirList()
: controller() { }

/**
 * @brief begin listing 
 * 
 * @param thread 
 * @param pathPtr 
 * @return int 
 */
int dirList::beginListing(Thread* thread, ak::uint32_t pathPtr) {
    char* path = (char*)pathPtr;
    if(requestBusy) {
        waitingQueue.push_back(thread);
    }

    requestBusy = true;
    currentReqThread = thread;

    return currentDirList->size();
}

/**
 * @brief get entry object
 * 
 * @param thread 
 * @param entry 
 * @param bufPtr 
 * @return int 
 */
int dirList::getEntry(Thread* thread, int entry, ak::uint32_t bufPtr) {
    char* buf = (char*)bufPtr;

    if (currentReqThread != thread) {
        return 0;
    }

    return 0;
}

/**
 * @brief end listing
 * 
 * @param thread 
 */
void dirList::endListing(Thread* thread) {
    if(currentReqThread != thread || currentReqThread == 0) {
        return;
    }

    requestBusy = false;
    currentReqThread = 0;
    delete currentDirList;
}