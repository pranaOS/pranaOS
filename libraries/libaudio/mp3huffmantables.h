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

    struct HuffmanTreeXY
    {
        Span<HuffmanNode<HuffmanXY> const> nodes;
        int linbits;
    }; // struct HuffmanTreeXY

    Array<HuffmanTreeXY const, 32> HuffmanTreesXY { {
        { Tree0, 0 },
        { Tree1, 0 },
        { Tree2, 0 },
        { Tree3, 0 },
        { {}, 0 },
        { Tree5, 0 },
        { Tree6, 0 },
        { Tree7, 0 },
        { Tree8, 0 },
        { Tree9, 0 },
        { Tree10, 0 },
        { Tree11, 0 },
        { Tree12, 0 },
        { Tree13, 0 },
        { {}, 0 },
        { Tree15, 0 },
        { Tree16, 1 },
        { Tree17, 2 },
        { Tree18, 3 },
        { Tree19, 4 },
        { Tree20, 6 },
        { Tree21, 8 },
        { Tree22, 10 },
        { Tree23, 13 },
        { Tree24, 4 },
        { Tree25, 5 },
        { Tree26, 6 },
        { Tree27, 7 },
        { Tree28, 8 },
        { Tree29, 9 },
        { Tree30, 11 },
        { Tree31, 13 },
    } };

} // namespace Audio::MP3::Tables::Huffman
