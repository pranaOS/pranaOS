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

    #define BUTTON_LEFT    0
    #define BUTTON_RIGHT   1
    #define BUTTON_MIDDLE  2

    #define PATH_SIZE               10 
    #define USAGE_TAB_SIZE          50 
    #define MAX_REPORT             300 

    #define SIZE_0                0x00
    #define SIZE_1                0x01
    #define SIZE_2                0x02
    #define SIZE_4                0x03
    #define SIZE_MASK             0x03
                                
    #define TYPE_MAIN             0x00
    #define TYPE_GLOBAL           0x04
    #define TYPE_LOCAL            0x08
    #define TYPE_MASK             0xC0

    #define ITEM_MASK             0xFC
    #define ITEM_UPAGE            0x04
    #define ITEM_USAGE            0x08  
    #define ITEM_LOG_MIN          0x14
    #define ITEM_USAGE_MIN        0x18  
    #define ITEM_LOG_MAX          0x24
    #define ITEM_USAGE_MAX        0x28  
    #define ITEM_PHY_MIN          0x34
    #define ITEM_PHY_MAX          0x44
    #define ITEM_UNIT_EXP         0x54
    #define ITEM_UNIT             0x64
    #define ITEM_REP_SIZE         0x74
    #define ITEM_STRING           0x78  
    #define ITEM_REP_ID           0x84
    #define ITEM_REP_COUNT        0x94

    #define ITEM_COLLECTION       0xA0
    #define ITEM_END_COLLECTION   0xC0
    #define ITEM_FEATURE          0xB0
    #define ITEM_INPUT            0x80
    #define ITEM_OUTPUT           0x90

    #define ATTR_DATA_CST         0x01
    #define ATTR_NVOL_VOL         0x80

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