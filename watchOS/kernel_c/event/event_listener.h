/**
 * @file event_listener.h
 * @author Krisna Pranav
 * @brief Event Listener
 * @version 1.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <Arduino.h>

class EventListener
{
  public:
    virtual bool listen(uint8_t event);
    virtual void handle();
};