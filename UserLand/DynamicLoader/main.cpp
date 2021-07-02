/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <LibC/sys/internals.h>
#include <LibC/unistd.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibELF/DynamicLinker.h>

char* __static_environ[] = { nullptr }; 

static void init_libc()
{
    environ = __static_environ;
    __environ_is_malloced = false;
    __stdio_is_initialized = false;
    __libc_init();
}

static void perform_self_relocations(auxv_t* auxvp)
{

    FlatPtr base_address = 0;
    bool found_base_address = false;
    for (; auxvp->a_type != AT_NULL; ++auxvp) {
        if (auxvp->a_type == ELF::AuxiliaryValue::BaseAddress) {
            base_address = auxvp->a_un.a_val;
            found_base_address = true;
        }
    }
    VERIFY(found_base_address);
    Elf32_Ehdr* header = (Elf32_Ehdr*)(base_address);
    Elf32_Phdr* pheader = (Elf32_Phdr*)(base_address + header->e_phoff);
    u32 dynamic_section_addr = 0;
    for (size_t i = 0; i < (size_t)header->e_phnum; ++i, ++pheader) {
        if (pheader->p_type != PT_DYNAMIC)
            continue;
        dynamic_section_addr = pheader->p_vaddr + base_address;
    }
    if (!dynamic_section_addr)
        exit(1);

    auto dynamic_object = ELF::DynamicObject::create({}, (VirtualAddress(base_address)), (VirtualAddress(dynamic_section_addr)));

    dynamic_object->relocation_section().for_each_relocation([base_address](auto& reloc) {
        if (reloc.type() != R_386_RELATIVE)
            return IterationDecision::Continue;

        *(u32*)reloc.address().as_ptr() += base_address;
        return IterationDecision::Continue;
    });
}

static void display_help()
{
    const char message[] =
        R"(You have invoked `Loader.so'. This is the helper program for programs that
use shared libraries. Special directives embedded in executables tell the
kernel to load this program.

This helper program loads the shared libraries needed by the program,
prepares the program to run, and runs it. You do not need to invoke
this helper program directly.
)";
    fprintf(stderr, "%s", message);
}

extern "C" {


void _start(int, char**, char**);

void _start(int argc, char** argv, char** envp)
{
    char** env;
    for (env = envp; *env; ++env) {
    }

    auxv_t* auxvp = (auxv_t*)++env;
    perform_self_relocations(auxvp);
    init_libc();

    int main_program_fd = -1;
    String main_program_name;
    bool is_secure = false;
    for (; auxvp->a_type != AT_NULL; ++auxvp) {
        if (auxvp->a_type == ELF::AuxiliaryValue::ExecFileDescriptor) {
            main_program_fd = auxvp->a_un.a_val;
        }
        if (auxvp->a_type == ELF::AuxiliaryValue::ExecFilename) {
            main_program_name = (const char*)auxvp->a_un.a_ptr;
        }
        if (auxvp->a_type == ELF::AuxiliaryValue::Secure) {
            is_secure = auxvp->a_un.a_val == 1;
        }
    }

    if (main_program_name == "/usr/lib/Loader.so") {
        display_help();
        _exit(1);
    }

    VERIFY(main_program_fd >= 0);
    VERIFY(!main_program_name.is_empty());

    ELF::DynamicLinker::linker_main(move(main_program_name), main_program_fd, is_secure, argc, argv, envp);
    VERIFY_NOT_REACHED();
}

void _fini();

void _fini()
{
}
}
