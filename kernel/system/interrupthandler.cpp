//
//  interrupthandler.cpp
//  pranaOS
//
//  Created by Krisna Pranav on 11/01/22.
//

#include "interrupthandler.h"

using namespace pranaOS;
using namespace pranaOS::ak;
using namespace pranaOS::system;


interruptHandler::interruptHandler(ak::uint8_t interruptNumber) {
    interruptManager::addHandler(this, interruptNumber);
}

uint32_t interruptHandler::handleInterrupt(uint32_t esp) {
    return esp;
}

List<interruptHandler*>* interruptManager::interruptCallbacks[256];
void interruptManager::initialize() {
    for(int i = 0; i < 256; i++)
    {
        interruptCallbacks[i] = 0;
    }
}

uint32_t interruptManager::handleInterrupt(uint8_t num, uint32_t esp) {
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

void interruptManager::addHandler(interruptHandler* handler, uint8_t interrupt) {
    if(interruptCallbacks[interrupt] == 0)
        interruptCallbacks[interrupt] = new List<interruptHandler*>();
    
    interruptCallbacks[interrupt]->push_back(handler);
}

void interruptManager::removeHandler(interruptHandler* handler, uint8_t interrupt) {
    interruptCallbacks[interrupt]->Remove(handler);
    if(interruptCallbacks[interrupt]->size() == 0) {
        delete interruptCallbacks[interrupt];
        interruptCallbacks[interrupt] = 0;
    }
}
