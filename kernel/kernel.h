//
// Created by Krisna Pranav on 18/01/22.
//

#pragma once

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