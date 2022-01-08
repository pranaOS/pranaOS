#pragma once

#ifndef __KERNEL__
#include <ak/types.h>
#else
#include <types.h>
#endif

namespace pranaOSsystemInfo {
    #define SYSTEM_INFO_ADDR 0xBFFEE000

    enum siPropertyIdentifer {
        String,
        Index
    };

    class siPropertyProvider {
    public:
        char* id = 0;
        int index = 0;
        siPropertyIdentifer type = String;
        siPropertyIdentifer* parent = 0;
    public:
        siPropertyProvider(char* property);
        siPropertyProvider(int index);

        int size();

        siPropertyProvider operator[](const int index);
        siPropertyProvider operator[](const char* element);

        template <typename T>
        T receiveValue(const bool getsize) const 

        template <typename T>
        T receiveBuffer() const;

        operator int() const;
        operator bool() const;
        operator double() const;
        operator char*() const;
        operator uint64_t() const;
        operator uint32_t() const;
        operator uint16_t() const;
        operator uint8_t() const;
    } __attribute__((packed));

    class systemInfo {
    public:
        static siPropertyProvider properties;
        static bool requestSystemInfo();  
    };

    struct sharedSystemInfo {
        unsigned int mousex;
        unsigned int mousey;
        signed int mousez;

        bool mouseLeftButton;
        bool mouseRightButton;
        bool mouseScroolButton;
    } __attribute__((packed));
}