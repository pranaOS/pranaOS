#pragma once

#include <ak/types.h>

namespace Kernel {

    enum HID_USAGE {
        POINSTER = 1,
        MOUSE,
        POINTER_X = 0x30,
        POINTER_Y,
        POINTER_WHEEL = 0x38,
        NOTHING = 0xFF   
    }

    enum HID_PAGE_USAGE {
        GEN_DESKTOP = 1,
        BUTTON = 9,
    };

    #define BUTTON_LEFT 0
    #define BUTTON_RIGHT 1
    #define BUTTON_MIDDLe 2

    #define PATH_SIZE 10
    #define USAGE_TAB_SIZE 50
    #define MAX_REPORT 300

    struct HID_NODE {
        ak::uint16_t u_page;
        ak::uint16_t usage;
    };

    struct HID_DATA {
        ak::uint32_t value;
        struct HID_PATH path;

        ak::uint8_t report_id;
        int report_count;
        int offset;
        int size;

        ak::uint8_t type;
        ak::uint8_t attribute;
        
        int log_min;
        int log_max;
        int phy_min;
        int phy_max;
    };

    class hidParser {
      private:
        hidParser();
    };
}