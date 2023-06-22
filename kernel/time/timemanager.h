/**
* @file timemanager.h
* @author Krisna Pranav
* @brief TimeManager
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/circular_queue.hpp"
#include "../kstd/unix_types.h"
#include "timekeeper.h"

class TimeManager {
  public:
    static void init();
    static TimeManager& inst();

    static timespec uptime();
    static timespec now();
    static double percent_idle();

  protected:
    friend class TimeKeeper;
    void tick();

  private:
    static TimeManager* _inst;
    TimeKeeper* _keeper = nullptr;
    timespec _epoch = {0, 0};
    timespec _uptime = {0, 0};
    int _ticks = 0;
    time_t _boot_epoch = 0;
    uint64_t _tsc_speed = 0;
    kstd::circular_queue<bool> idle_ticks = kstd::circular_queue<bool>(100);
};

extern "C" void __attribute((cdecl)) measure_tsc_speed();
