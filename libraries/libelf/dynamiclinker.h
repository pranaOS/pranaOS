/**
 * @file dynamiclinker.h
 * @author your name (you@domain.com)
 * @brief Dynamic Linker
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/result.h>
#include <mods/vector.h>
#include <libelf/dynamicobject.h>

namespace ELF 
{
    class DynamicLinker 
    {
    public:
        /**
         * @param symbol 
         * @return Optional<DynamicObject::SymbolLookupResult> 
         */
        static Optional<DynamicObject::SymbolLookupResult> lookup_global_symbol(StringView symbol);

        /**
         * @param main_program_name 
         * @param fd 
         * @param is_secure 
         * @param argc 
         * @param argv 
         * @param envp 
         */
        [[noreturn]] static void linker_main(String&& main_program_name, int fd, bool is_secure, int argc, char** argv, char** envp);

    private:
        /**
         * @brief Construct a new Dynamic Linker object
         * 
         */
        DynamicLinker() = delete;

        /**
         * @brief Destroy the Dynamic Linker object
         * 
         */
        ~DynamicLinker() = delete;
    }; // namespace DynamicLinker
} // namespace ELF