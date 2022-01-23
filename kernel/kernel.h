//
// Created by Krisna Pranav on 18/01/22.
//

#pragma once

#include "console.h"
#include <kernel/internal/systemcomponent.h>
#include <kernel/internal/rtc.h>
#include <kernel/internal/smbios.h>
#include <kernel/internal/graphics.h>

namespace Kernel {
    #define DEFAULT_SCREEN_WIDTH 1024
    #define DEFAULT_SCREEN_HEIGHT 876
    #define DEFAULT_SCREEN_BPP 32

    enum screenMode {
        TextMode,
        GraphicsMode,
    };

    enum powerRequest {
        None,
        Shutdown,
        Reboot
    };

    class system {
        static void start();
        static void panic();
    };
}