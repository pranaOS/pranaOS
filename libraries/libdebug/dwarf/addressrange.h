/**
 * @file addressrange.h
 * @author Krisna Pranav
 * @brief address range
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "compilationunit.h"
#include <mods/forward.h>
#include <mods/function.h>
#include <mods/memorystream.h>
#include <mods/noncopyable.h>

namespace Debug::Dwarf 
{

    struct Range {
        FlatPtr start { 0 };
        FlatPtr end { 0 };
    }; // struct Range

    class AddressRangesV5 
    {
        MOD_MAKE_NONCOPYABLE(AddressRangesV5);
        MOD_MAKE_NONMOVABLE(AddressRangesV5);

    public:
        /**
         * @brief Construct a new Address RangesV5 object
         * 
         * @param range_lists_data 
         * @param offset 
         * @param compilation_unit 
         */
        AddressRangesV5(ReadonlyBytes range_lists_data, size_t offset, CompilationUnit const& compilation_unit);

        void for_each_range(Function<void(Range)>);

    private:
        InputMemoryStream m_range_lists_stream;
        CompilationUnit const& m_compilation_unit;
    }; // class AddressRangesV5 

    class AddressRangesV4 
    {
        MOD_MAKE_NONCOPYABLE(AddressRangesV4);
        MOD_MAKE_NONMOVABLE(AddressRangesV4);

    public:
        /**
         * @brief Construct a new AddressRangesV4 object
         * 
         * @param ranges_data 
         * @param offset 
         */
        AddressRangesV4(ReadonlyBytes ranges_data, size_t offset, CompilationUnit const&);

        void for_each_range(Function<void(Range)>);

    private:
        InputMemoryStream m_ranges_stream;
        CompilationUnit const& m_compilation_unit;
    }; // class AddressRangesV4 

} // namespace Debug::Dwarf
