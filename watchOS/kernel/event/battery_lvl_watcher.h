/**
* @file battery_lvl_watcher
* @author Krisna Pranav
* @brief Battery Level Watcher
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */


#pragma once

#include "event_listener.h"

class BatteryLevelWarningListener : public EventListener {
  public:
    virtual bool listen(uint8_t event);

    virtual void handle();

    BatteryLevelWarningListener() {}

  protected:
    uint8_t batteryLowWarnVibrateOnLevel = 101;
};