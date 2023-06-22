/**
* @file blocker.h
* @author Krisna Pranav
* @brief Blocker
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
*/

#pragma once

class Process;
class Thread;

class Blocker {
  public:
    virtual bool is_ready() = 0;
    virtual bool can_be_interrupted();
    virtual bool is_lock();
    virtual Thread* responsible_thread();

    void interrupt();
    void reset_interrupted();
    bool was_interrupted();

  protected:
    virtual void on_interrupted();

  private:
    bool _interrupted = false;
};