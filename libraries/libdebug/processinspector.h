/**
 * @file processinspector.h
 * @author Krisna Pranav
 * @brief process inspector
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "loadedlibrary.h"
#include <mods/types.h>
#include <libc/sys/arch/i386/regs.h>

namespace Debug 
{

    class ProcessInspector 
    {
    public:
        /**
         * @brief Destroy the Process Inspector object
         * 
         */
        virtual ~ProcessInspector() = default;

        /**
         * @param address 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool poke(FlatPtr address, FlatPtr data) = 0;

        /**
         * @param address 
         * @return Optional<FlatPtr> 
         */
        virtual Optional<FlatPtr> peek(FlatPtr address) const = 0;

        virtual PtraceRegisters get_registers() const = 0;

        virtual void set_registers(PtraceRegisters const&) = 0;

        virtual void for_each_loaded_library(Function<IterationDecision(LoadedLibrary const&)>) const = 0;

        /**
         * @param address 
         * @return LoadedLibrary const* 
         */
        LoadedLibrary const* library_at(FlatPtr address) const;

        struct SymbolicationResult {
            String library_name;
            String symbol;
        }; // struct SymbolicationResult
 
        /**
         * @param address 
         * @return Optional<SymbolicationResult> 
         */
        Optional<SymbolicationResult> symbolicate(FlatPtr address) const;

        /**
         * @brief Get the address from source position object
         * 
         * @param file 
         * @param line 
         * @return Optional<DebugInfo::SourcePositionAndAddress> 
         */
        Optional<DebugInfo::SourcePositionAndAddress> get_address_from_source_position(String const& file, size_t line) const;

        /**
         * @brief Get the source position object
         * 
         * @param address 
         * @return Optional<DebugInfo::SourcePosition> 
         */
        Optional<DebugInfo::SourcePosition> get_source_position(FlatPtr address) const;

    protected:
        /**
         * @brief Construct a new ProcessInspector object
         * 
         */
        ProcessInspector() = default;
    }; // class ProcessInspector 

}; // namespace Debug
