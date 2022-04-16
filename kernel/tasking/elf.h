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
        uint16_t etype;
        uint16_t emachine;
        uint16_t ehsize;
        uint16_t ephentsize;
        uint16_t ephnum;
        uint16_t eshentsize;
        uint16_t eshnum;
        uint16_t eshstrndx;
        uint32_t eversion;
        uint32_t entry;
        uint32_t ephoff;
        uint32_t eshoff;
        uint32_t eflags;
    } __attribute__((packed));

    struct elfSectionHeader {
        uint32_t shname;
        uint32_t shtype;
        uint32_t shflags;
        uint32_t shaddr;
        uint32_t shoffset;
        uint32_t shsize;
        uint32_t shlink;
        uint32_t shinfo;
        uint32_t shaddralign;
        uint32_t shentsize;
    } __attribute__((packed));

    struct elfProgramHeader {
        uint32_t ptype;
        uint32_t poffset;
        uint32_t pvaddr;
        uint32_t ppaddr;
        uint32_t pfilesz;
        uint32_t pmemsz;
        uint32_t pflags;
        uint32_t palign;
    } __attribute__((packed));

}