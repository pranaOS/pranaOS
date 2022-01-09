#include <system/console.h>

using namespace pranaOSConsole;
using namespace Kernel::system;
using namespace Kernel::ak;
using namespace Kernel;

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

void bootConsole::init(bool enableSerial) {    
    bootConsole::writeToSerial = enableSerial;

    if (enableSerial) {
        Serialport::Init(COMPort::COM1);
        bootConsole::writeLine()
    }
}

void bootConsole::write(char c) {
    static char* str = " ";
    str[0] = c;
    write(str);
}

void bootConsole::write(char* str) {
    if (writeToSerial)
        Serialport::WriteStr(str);

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                XOffset = 0;
                YOffset++;
                break;
            case '\t':
                Write("    "); //4 spaces for tab
                break;
            default:
                uint16_t attrib = (BackgroundColor << 4) | (ForegroundColor & 0x0F);
                volatile uint16_t * where;
                where = (volatile uint16_t *)videoMemory + (YOffset * 80 + XOffset) ;
                *where = str[i] | (attrib << 8);
                XOffset++;
                break;
        }

        if(XOffset >= 80)
        {
            XOffset = 0;
            YOffset++;
        }

        if(YOffset >= 25)
        {
            Scroll();
            XOffset = 0;
            YOffset = 24;
        }
    }
}
void BootConsole::WriteLine(char* str)
{
    BootConsole::Write(str);
    BootConsole::Write("\n");
}