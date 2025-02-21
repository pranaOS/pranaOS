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

    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table25 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table26 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table27 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table28 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table29 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table30 { Table24 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table31 { Table24 };

    constexpr auto TreeA = make_huffman_tree<TableA>();
    constexpr auto TreeB = make_huffman_tree<TableB>();

    constexpr auto Tree0 = make_huffman_tree<Table0>();
    constexpr auto Tree1 = make_huffman_tree<Table1>();
    constexpr auto Tree2 = make_huffman_tree<Table2>();
    constexpr auto Tree3 = make_huffman_tree<Table3>();
    constexpr auto Tree5 = make_huffman_tree<Table5>();
    constexpr auto Tree6 = make_huffman_tree<Table6>();
    constexpr auto Tree7 = make_huffman_tree<Table7>();
    constexpr auto Tree8 = make_huffman_tree<Table8>();
    constexpr auto Tree9 = make_huffman_tree<Table9>();
    constexpr auto Tree10 = make_huffman_tree<Table10>();
    constexpr auto Tree11 = make_huffman_tree<Table11>();
    constexpr auto Tree12 = make_huffman_tree<Table12>();
    constexpr auto Tree13 = make_huffman_tree<Table13>();
    constexpr auto Tree15 = make_huffman_tree<Table15>();
    constexpr auto Tree16 = make_huffman_tree<Table16>();
    constexpr auto Tree17 = make_huffman_tree<Table17>();
    constexpr auto Tree18 = make_huffman_tree<Table18>();
    constexpr auto Tree19 = make_huffman_tree<Table19>();
    constexpr auto Tree20 = make_huffman_tree<Table20>();
    constexpr auto Tree21 = make_huffman_tree<Table21>();
    constexpr auto Tree22 = make_huffman_tree<Table22>();
    constexpr auto Tree23 = make_huffman_tree<Table23>();
    constexpr auto Tree24 = make_huffman_tree<Table24>();
    constexpr auto Tree25 = make_huffman_tree<Table25>();
    constexpr auto Tree26 = make_huffman_tree<Table26>();
    constexpr auto Tree27 = make_huffman_tree<Table27>();
    constexpr auto Tree28 = make_huffman_tree<Table28>();
    constexpr auto Tree29 = make_huffman_tree<Table29>();
    constexpr auto Tree30 = make_huffman_tree<Table30>();
    constexpr auto Tree31 = make_huffman_tree<Table31>();


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
