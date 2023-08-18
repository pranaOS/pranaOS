/**
 * @file timemanager.cpp
 * @author Krisna Pranav
 * @brief TimeManager
 * @version 6.0
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "timemanager.h"
#include "PIT.h"
#include "RTC.h"

TimeManager* TimeManager::_inst = nullptr;

void TimeManager::init() {
    if(_inst)
        return;
    
    _inst = new TimeManager();
    _inst->_keeper->enable();
}