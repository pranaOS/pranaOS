#include <system/console.h>

using namespace pranaOSConsole;
using namespace pranaOS::ak;
using namespace pranaOS::system;

int bootConsole::xOffset = 0;
int bootConsole::yOffset = 0;
uint8_t bootConsole::backgroundcolor = VGA_COLOR_BLACK;
uint8_t bootConsole::foregroundcolor = VGA_COLOR_WHITE;
bool bootConsole::writeToSerial = false;

static uint16_t* videoMemory = (uint16_t*)0xC00B8000;

void bootSonsole::scroll() {
    for(int i = 0; i < 24; i++){
        for (int m = 0; m < 80; m++){
            videoMemory[i * 80 + m] = videoMemory[(i + 1) * 80 + m];
        }
    }

    for(int x = 0; x < 80; x++) {
        uint16_t attrib = (backgroundcolor << 4) | (foregroundcolor & 0x0F);
        volatile uint16_t * where;
        where = (volatile uint16_t *)videoMemory + (24 * VGA_WIDTH + x) ;
        *where = ' ' | (attrib << 8);
    }
}