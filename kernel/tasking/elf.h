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
        ak::uint32_t eversion;
        ak::uint32_t entry;
        ak::uint32_t ephoff;
        ak::uint32_t eshoff;
        ak::uint32_t eflags;
        ak::uint16_t ehsize;
        ak::uint16_t ephentsize;
        ak::uint16_t ephnum;
        ak::uint16_t eshentsize;
        ak::uint16_t eshnum;
        ak::uint16_t eshstrndx;
    } __attribute__((packed));

    struct elfSectionHeader {

    };

}