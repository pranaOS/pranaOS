/**
 * @file processinspector.cpp
 * @author Krisna Pranav
 * @brief process inspector
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "processinspector.h"
#include "debuginfo.h"

namespace Debug 
{

    /**
     * @param address 
     * @return LoadedLibrary const* 
     */
    LoadedLibrary const* ProcessInspector::library_at(FlatPtr address) const
    {
        LoadedLibrary const* result = nullptr;
        for_each_loaded_library([&result, address](auto const& lib) {
            if (address >= lib.base_address && address < lib.base_address + lib.debug_info->elf().size()) {
                result = &lib;
                return IterationDecision::Break;
            }
            return IterationDecision::Continue;
        });
        return result;
    }

    /**
     * @param address 
     * @return Optional<ProcessInspector::SymbolicationResult> 
     */
    Optional<ProcessInspector::SymbolicationResult> ProcessInspector::symbolicate(FlatPtr address) const
    {
        auto* lib = library_at(address);
        if (!lib)
            return {};
        
        auto symbol = lib->debug_info->elf().symbolicate(address - lib->base_address);
        return { { lib->name, symbol } };
    }

    /**
     * @param file 
     * @param line 
     * @return Optional<DebugInfo::SourcePositionAndAddress> 
     */
    Optional<DebugInfo::SourcePositionAndAddress> ProcessInspector::get_address_from_source_position(String const& file, size_t line) const
    {
        Optional<DebugInfo::SourcePositionAndAddress> result;
        for_each_loaded_library([file, line, &result](auto& lib) {
            if (lib.name == "Loader.so")
                return IterationDecision::Continue;

            auto source_position_and_address = lib.debug_info->get_address_from_source_position(file, line);
            if (!source_position_and_address.has_value())
                return IterationDecision::Continue;

            result = source_position_and_address;
            result.value().address += lib.base_address;
            return IterationDecision::Break;
        });
        return result;
    }

    /**
     * @param address 
     * @return Optional<DebugInfo::SourcePosition> 
     */
    Optional<DebugInfo::SourcePosition> ProcessInspector::get_source_position(FlatPtr address) const
    {
        auto* lib = library_at(address);
        if (!lib)
            return {};
        return lib->debug_info->get_source_position(address - lib->base_address);
    }

} // namespace Debug
