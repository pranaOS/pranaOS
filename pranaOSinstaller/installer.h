//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

namespace pranaOSInstaller {
    class Installer {
      public:
        static void setupError();
        static void run();
        static void getKey();
        static void showWelcomeMessage();
        static void showWarningMessage();
        static void showDiskSelection();
        static void showInstallScreen();
        static void showSystemCopyScreen();
        static void createFatPartition();
    };

    #define KEY_ENTRY 0x1C
    #define KEY_ARROW_UP 0x48
    #define KEY_ARROW_DOWN 0x50
}