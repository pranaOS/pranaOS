/**
* @file timekeeper.h
* @author Krisna Pranav
* @brief TimeKeeper
* @version 1.0
*
* @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/kstddef.h"

class TimeManager;

class TimeKeeper {
  public:
    explicit TimeKeeper(TimeManager* time);
    virtual int frequency() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;

  protected:
    void tick();

  private:
    TimeManager* _manager;
};