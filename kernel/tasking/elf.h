//
// Created by KrisnaPranav on 23/01/22.
//

#pragma once

#include <kernel/tasking/process.h>
#include <ak/types.h>

namespace Kernel {
    #define ELFMAG0 0x7f
    #define ELFMAG1 'E'
    #define ELFMAG2 'L'
    #define ELFMAG3 'F'

    struct elfHeader {
        unsigned char eident[16];
        ak::uint16_t etype;
        ak::uint16_t emachine;
        ak::uint16_t ehsize;
        ak::uint16_t ephentsize;
        ak::uint16_t ephnum;
        ak::uint16_t eshentsize;
        ak::uint16_t eshnum;
        ak::uint16_t eshstrndx;
        ak::uint32_t eversion;
        ak::uint32_t entry;
        ak::uint32_t ephoff;
        ak::uint32_t eshoff;
        ak::uint32_t eflags;
    } __attribute__((packed));

    struct elfSectionHeader {
        ak::uint32_t shname;
        ak::uint32_t shtype;
        ak::uint32_t shflags;
        ak::uint32_t shaddr;
        ak::uint32_t shoffset;
        ak::uint32_t shsize;
        ak::uint32_t shlink;
        ak::uint32_t shinfo;
        ak::uint32_t shaddralign;
        ak::uint32_t shentsize;
    } __attribute__((packed));

}