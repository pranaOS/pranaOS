/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <interrupthandler.h>

using namespace Kernel;

/**
 * @brief Construct a new interrupt Handler::interrupt Handler object
 * 
 * @param interruptNumber 
 */
interruptHandler::interruptHandler(ak::uint8_t interruptNumber) {
    interruptManager::addHandler(this, interruptNumber);
}

/**
 * @brief handleInterrupt 
 * 
 * @param esp 
 * @return ak::uint32_t 
 */
ak::uint32_t interruptHandler::handleInterrupt(ak::uint32_t esp) {
    return esp;
}

/**
 * @brief Interruptmanager
 * 
 */
ak::List<interruptHandler*>* interruptManager::interruptCallbacks[256];

/**
 * @brief initialize function
 * 
 */
void interruptManager::initialize() {
    for(int i = 0; i < 256; i++)
    {
        interruptCallbacks[i] = 0;
    }
}

/**
 * @brief handle interrupts
 * 
 * @param num 
 * @param esp 
 * @return ak::uint32_t 
 */
uint32_t interruptManager::handleInterrupt(ak::uint8_t num, ak::uint32_t esp) {
    if(interruptCallbacks[num] == 0)
        return esp;
    
    if(interruptCallbacks[num]->size() == 0)
        return esp;
    
    for(int i = 0; i < interruptCallbacks[num]->size(); i++)
    {
        interruptHandler* handler = interruptCallbacks[num]->GetAt(i);
        esp = handler->handleInterrupt(esp);
    }

    return esp;
}

/**
 * @brief add handler
 * 
 * @param handler 
 * @param interrupt 
 */
void interruptManager::addHandler(interruptHandler* handler, ak::uint8_t interrupt) {
    if(interruptCallbacks[interrupt] == 0)
        interruptCallbacks[interrupt] = new ak::List<interruptHandler*>();
    
    interruptCallbacks[interrupt]->push_back(handler);
}

/**
 * @brief remove handler
 * 
 * @param handler 
 * @param interrupt 
 */
void interruptManager::removeHandler(interruptHandler* handler, ak::uint8_t interrupt) {
    interruptCallbacks[interrupt]->Remove(handler);
    if(interruptCallbacks[interrupt]->size() == 0) {
        delete interruptCallbacks[interrupt];
        interruptCallbacks[interrupt] = 0;
    }
}
