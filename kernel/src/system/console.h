#pragma once

#include <ak/types.h>
#include <system/serialport.h>
#include <stdarg.h>

namespace Kernel {
    namespace system {
        #define VGA_WIDTH 80
        #define VGA_HEIGHT 25

        enum vga_color {
            VGA_COLOR_BLACK = 0,
            VGA_COLOR_BLUE = 1,
            VGA_COLOR_GREEN = 2,
            VGA_COLOR_CYAN = 3,
            VGA_COLOR_RED = 4,
            VGA_COLOR_MAGENTA = 5,
            VGA_COLOR_BROWN = 6,
            VGA_COLOR_LIGHT_GREY = 7,
            VGA_COLOR_DARK_GREY = 8,
            VGA_COLOR_LIGHT_BLUE = 9,
            VGA_COLOR_LIGHT_GREEN = 10,
            VGA_COLOR_LIGHT_CYAN = 11,
            VGA_COLOR_LIGHT_RED = 12,
            VGA_COLOR_LIGHT_MAGENTA = 13,
            VGA_COLOR_LIGHT_BROWN = 14,
            VGA_COLOR_WHITE = 15
        };

        class bootConsole {
        public:
            static ak::uint8_t foregroundcolor;
            static ak::uint8_t backgroundcolor;

            static void init(bool enableSerial = false);

            static void write(char c);
            static void write(char* str);

            static void writeLine(char* str);
            static void writeLine();

            static void clear();
            static void setx(int x);
            static void sety(int y);

            static ak::uint16_t* getBuffer();
        
        private:
            static int xoffset;
            static int yoffset;
            static bool writeToSerial;

            static void scroll();
        };
    }
}