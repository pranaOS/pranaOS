#include "console.h"

using namespace Kernel;
using namespace Kernel::ak;

int bootConsole::xoffset = 0;
int bootConsole::yoffset = 0;
uint8_t bootConsole::backgroundcolor = VGA_COLOR_BLACK; 
uint8_t bootConsole::foregroundcolor = VGA_COLOR_WHITE; 
bool bootConsole::writeToSerial = false;

static uint16_t* videoMemory = (uint16_t*)0xC00B8000;

void bootConsole::scroll() {
    for(int i = 0; i < 24; i++){
        for (int m = 0; m < 80; m++){
            videoMemory[i * 80 + m] = videoMemory[(i + 1) * 80 + m];
        }
    }

    for(int x = 0; x < 80; x++)
    {
        uint16_t attrib = (backgroundcolor << 4) | (foregroundcolor & 0x0F);
        volatile uint16_t * where;
        where = (volatile uint16_t *)videoMemory + (24 * VGA_WIDTH + x) ;
        *where = ' ' | (attrib << 8);
    }
}

void bootConsole::init(bool enableSerial) {
    bootConsole::writeToSerial = enableSerial;
    if(enableSerial)
    {
        serialPort::init(COMPort::COM1);
        bootConsole::writeLine("Start of serial log for pranaOS");
    }
}

void bootConsole::write(char c) {
    static char* str = " ";
    str[0] = c;
    write(str);
}

void bootConsole::write(char* str) {
    if (writeToSerial)
        serialPort::writeStr(str);

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                xoffset = 0;
                yoffset++;
                break;
            case '\t':
                write("    "); 
                break;
            default:
                uint16_t attrib = (backgroundcolor << 4) | (foregroundcolor & 0x0F);
                volatile uint16_t * where;
                where = (volatile uint16_t *)videoMemory + (yoffset * 80 + xoffset) ;
                *where = str[i] | (attrib << 8);
                xoffset++;
                break;
        }

        if(xoffset >= 80) {
            xoffset = 0;
            yoffset++;
        }

        if(yoffset >= 25) {
            scroll();
            xoffset = 0;
            yoffset = 24;
        }
    }
}

void bootConsole::writeLine(char* str) {
    bootConsole::write(str);
    bootConsole::write("\n");
}

void bootConsole::writeLine() {
    bootConsole::write("\n");
}

void bootConsole::clear() {
    for(int y = 0; y < VGA_HEIGHT; y++)
        for(int x = 0; x < VGA_WIDTH; x++) {
                uint16_t attrib = (backgroundcolor << 4) | (foregroundcolor & 0x0F);
                volatile uint16_t * where;
                where = (volatile uint16_t *)videoMemory + (y * VGA_WIDTH + x) ;
                *where = ' ' | (attrib << 8);
        }

    xoffset = 0;
    yoffset = 0;
}

uint16_t* bootConsole::getBuffer() {
    return videoMemory;
}

void bootConsole::setx(int x) {
    xoffset = x;
}

void bootConsole::sety(int y) {
    yoffset = y;
}
