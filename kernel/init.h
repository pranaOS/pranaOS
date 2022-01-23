#pragma once

#include <ak/string.h>
#include <ak/types.h>
#include <multiboot/multiboot.h>
#include <kernel/console.h>

namespace Kernel {
    struct init {
        char name[30];
        char path[100];
        ak::uint32_t size;
    } __attribute__((packed));

    class Intial {
    public:
        static void initialize(multiboot_info_t* mbi);
        static void* readFile(const char* path, ak::uint32_t* fileSizeReturn = 0);
    };
}