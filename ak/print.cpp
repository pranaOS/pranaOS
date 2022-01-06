#include "print.h"

using namespace pranaOS;
using namespace pranaOS::system;
using namespace pranaOS::ak;
using namespace pranaOS::core;

void Print::printfHex(uint8_t key) {
    char *foo = "00";
    char *hex = "0123456789";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    system::BootConsole::write(foo);
}

void Print::printfHex16(uint16_t key) {
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void Print::printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}