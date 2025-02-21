/**
 * @file mp3huffmantables.h
 * @author Krisna Pranav
 * @brief mp3 huff man tables
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/bitstream.h>
#include <mods/span.h>

namespace Audio::MP3::Tables::Huffman
{
    template<typename Symbol>
    struct HuffmanEntry
    {
        using SymbolType = Symbol;
        unsigned code;
        size_t code_length;
        Symbol symbol;
    }; // struct HuffmanEntry

    template<typename Symbol>
    struct HuffmanNode
    {
        int left; 
        int right;
        unsigned code;
        size_t code_length;
        Symbol symbol;

        bool is_leaf() const
        {
            return left == -1 && right == -1;
        }
    }; // struct HuffmanNode

    /**
     * @tparam Symbol 
     * @tparam Size 
     */
    template<typename Symbol, size_t Size>
    using HuffmanEntries = Array<Symbol, Size>;

    /**
     * @tparam Symbol 
     * @tparam Size 
     */
    template<typename Symbol, size_t Size>
    using HuffmanNodes = Array<HuffmanNode<Symbol>, Size>;

    /**
     * @tparam Symbol 
     * @tparam Size 
     * @param table 
     * @return consteval 
     */
    template<typename Symbol, size_t Size>
    consteval size_t length_of_longest_huffman_code(HuffmanEntries<Symbol, Size> const& table)
    {
        size_t max = 0;

        for(size_t i = 0; i < table.size(); i++) {
            if (table[i].code_length > max)
                max = table[i].code_length;
        }

        return max;
    }
} // namespace Audio::MP3::Tables::Huffman
