/**
 * @file battery_manager.hpp
 * @author Krisna Pranav
 * @brief BatteryManager
 * @version 6.0
 * @date 2023-06-18
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <libutils/config.h>

class BatteryManager
{
  public:
    static BatteryManager* getInstance();
    uint8_t getCapacity();
    bool isCharging();
    bool handleCablePlugInIRQ();
    bool handleCablePlugRemoveIRQ();
    void energyConsumptionSavingsSettings();
    void updateCapacity();
    uint8_t getUpdatedCapacity();

  protected:
    static BatteryManager* inst;
    int lastCapacity = 0;
    BatteryManager();
}
