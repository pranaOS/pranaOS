/**
* @file plug_listener.h
* @author Krisna Pranav
* @brief Plug Listener
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "event_listener.h"

class PlugListener : public EventListener {
  public:
    virtual bool listen(uint8_t event);

    virtual void handle();

    PlugListener() {}
};