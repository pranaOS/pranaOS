/**
* @file waitblocker.h
* @author Krisna Pranav
* @brief WaitBlocker
* @version 1.0
*
* @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "blocker.h"
#include "../kstd/unix_types.h"
#include "../kstd/arc.h"

class Thread;

class WaitBlocker: public Blocker {
  public:
    WaitBlocker(kstd::Arc<Thread> thread, pid_t wait_for);
    bool is_ready() override;

    pid_t waited_pid();
    Process* waited_process();
    pid_t error();
    pid_t exit_status();

  private:
    int _err = 0;
    int _exit_status = 0;
    Process* _waited_process = nullptr;
    pid_t _wait_pid;
    pid_t _wait_pgid;
    kstd::Arc<Thread> _thread;
};