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

// signal returns up fake addr
#define SIGNAL_RETURN_FAKE_ADDR 0xFFFFB00F

namespace Signal {
    class SigAction {
      public:
        void (*action)(int) = nullptr;
        int flags = 0;
    };

    enum SignalSeverity {
        NOKILL,
        KILL,
        FATAL
    };

    extern const char* signal_names[];

    extern SignalSeverity signal_severities[];
}