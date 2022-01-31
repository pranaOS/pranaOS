//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include <kernel/log.h>

namespace Kernel {
    class Installer {
      public:
        // const char void setupError();
        const char setupError();
        static void run();
        static void getKey();
        static void showWelcomeMessage();
        static void showWarningMessage();
        static void showDiskSelection();
        static void showInstallScreen();
        static void showSystemCopyScreen();
        static void createFatPartition();

      private:
        Installer();
        ~Installer();
    };

    #define KEY_ENTER       0x1C
    #define KEY_ARROW_UP    0x48
    #define KEY_ARROW_DOWN  0x50
    #define KEY_LEFT_ARROW  0x33
    #define KEY_RIGHT_ARROW 0x34
    #define KEY_MINUS       0x0C
    #define KEY_PLUS        0x0D
    #define KEY_A           0x1E
}

using Kernel::Installer;