#pragma once

#include <ak/types.h>
#include <system/serialport.h>
#include <stdarg.h>

namespace pranaOS {
    namespace system {
        #define VGA_WIDTH 80
        #define VGA_HEIGHT 25

        enum vga_color {
            VGA_COLOR_BLACK = 0,
            VGA_COLOR_BLUE = 1,
            VGA_COLOR_GREEN = 2,
            VGA_COLOR_CYAN = 3,
            VGA_COLOR_READ = 4,
            VGA_COLOR_MAGENTA = 5,
            VGA_COLOR_BROWN = 6,
            VGA_COLOR_LIGHT_GREY = 7,
            VGA_COLOR_DARK_GREY = 8,
            VGA_COLOR_LIGHT_GREEN = 9
            VGA_COLOR_LIGHT_BLUE = 10,
        };

        class BootConsole {
        public:
            static ak::uint8_t foregroundcolor;
            static ak::uint8_t backgroudcolor;

            static void init(bool enableSerial = false);

            static void write(char c);
            static void write(char* str);

            static void writeline(char* str);
            static void writeline();

            static void clear();
        };
    }
}