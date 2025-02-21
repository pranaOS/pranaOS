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

    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table17 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table18 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table19 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table20 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table21 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table22 { Table16 };
    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table23 { Table16 };

    constexpr HuffmanEntries<HuffmanEntryXY, 256> Table24 { {
        { 0, 0, 4, 0b1111 },
        { 0, 1, 4, 0b1101 },
        { 0, 2, 6, 0b101110 },
        { 0, 3, 7, 0b1010000 },
        { 0, 4, 8, 0b10010010 },
        { 0, 5, 9, 0b100000110 },
        { 0, 6, 9, 0b011111000 },
        { 0, 7, 10, 0b0110110010 },
        { 0, 8, 10, 0b0110101010 },
        { 0, 9, 11, 0b01010011101 },
        { 0, 10, 11, 0b01010001101 },
        { 0, 11, 11, 0b01010001001 },
        { 0, 12, 11, 0b01001101101 },
        { 0, 13, 11, 0b01000000101 },
        { 0, 14, 12, 0b010000001000 },
        { 0, 15, 9, 0b001011000 },
        { 1, 0, 4, 0b1110 },
        { 1, 1, 4, 0b1100 },
        { 1, 2, 5, 0b10101 },
        { 1, 3, 6, 0b100110 },
        { 1, 4, 7, 0b1000111 },
        { 1, 5, 8, 0b10000010 },
        { 1, 6, 8, 0b01111010 },
        { 1, 7, 9, 0b011011000 },
        { 1, 8, 9, 0b011010001 },
        { 1, 9, 9, 0b011000110 },
        { 1, 10, 10, 0b0101000111 },
        { 1, 11, 10, 0b0101011001 },
        { 1, 12, 10, 0b0100111111 },
        { 1, 13, 10, 0b0100101001 },
        { 1, 14, 10, 0b0100010111 },
        { 1, 15, 8, 0b00101010 },
        { 2, 0, 6, 0b101111 },
        { 2, 1, 5, 0b10110 },
        { 2, 2, 6, 0b101001 },
        { 2, 3, 7, 0b1001010 },
        { 2, 4, 7, 0b1000100 },
        { 2, 5, 8, 0b10000000 },
        { 2, 6, 8, 0b01111000 },
        { 2, 7, 9, 0b011011101 },
        { 2, 8, 9, 0b011001111 },
        { 2, 9, 9, 0b011000010 },
        { 2, 10, 9, 0b010110110 },
        { 2, 11, 10, 0b0101010100 },
        { 2, 12, 10, 0b0100111011 },
        { 2, 13, 10, 0b0100100111 },
        { 2, 14, 11, 0b01000011101 },
        { 2, 15, 7, 0b0010010 },
        { 3, 0, 7, 0b1010001 },
        { 3, 1, 6, 0b100111 },
        { 3, 2, 7, 0b1001011 },
        { 3, 3, 7, 0b1000110 },
        { 3, 4, 8, 0b10000110 },
        { 3, 5, 8, 0b01111101 },
        { 3, 6, 8, 0b01110100 },
        { 3, 7, 9, 0b011011100 },
        { 3, 8, 9, 0b011001100 },
        { 3, 9, 9, 0b010111110 },
        { 3, 10, 9, 0b010110010 },
        { 3, 11, 10, 0b0101000101 },
        { 3, 12, 10, 0b0100110111 },
        { 3, 13, 10, 0b0100100101 },
        { 3, 14, 10, 0b0100001111 },
        { 3, 15, 7, 0b0010000 },
        { 4, 0, 8, 0b10010011 },
        { 4, 1, 7, 0b1001000 },
        { 4, 2, 7, 0b1000101 },
        { 4, 3, 8, 0b10000111 },
        { 4, 4, 8, 0b01111111 },
        { 4, 5, 8, 0b01110110 },
        { 4, 6, 8, 0b01110000 },
        { 4, 7, 9, 0b011010010 },
        { 4, 8, 9, 0b011001000 },
        { 4, 9, 9, 0b010111100 },
        { 4, 10, 10, 0b0101100000 },
        { 4, 11, 10, 0b0101000011 },
        { 4, 12, 10, 0b0100110010 },
        { 4, 13, 10, 0b0100011101 },
        { 4, 14, 11, 0b01000011100 },
        { 4, 15, 7, 0b0001110 },
        { 5, 0, 9, 0b100000111 },
        { 5, 1, 7, 0b1000010 },
        { 5, 2, 8, 0b10000001 },
        { 5, 3, 8, 0b01111110 },
        { 5, 4, 8, 0b01110111 },
        { 5, 5, 8, 0b01110010 },
        { 5, 6, 9, 0b011010110 },
        { 5, 7, 9, 0b011001010 },
        { 5, 8, 9, 0b011000000 },
        { 5, 9, 9, 0b010110100 },
        { 5, 10, 10, 0b0101010101 },
        { 5, 11, 10, 0b0100111101 },
        { 5, 12, 10, 0b0100101101 },
        { 5, 13, 10, 0b0100011001 },
        { 5, 14, 10, 0b0100000110 },
        { 5, 15, 7, 0b0001100 },
        { 6, 0, 9, 0b011111001 },
        { 6, 1, 8, 0b01111011 },
        { 6, 2, 8, 0b01111001 },
        { 6, 3, 8, 0b01110101 },
        { 6, 4, 8, 0b01110001 },
        { 6, 5, 9, 0b011010111 },
        { 6, 6, 9, 0b011001110 },
        { 6, 7, 9, 0b011000011 },
        { 6, 8, 9, 0b010111001 },
        { 6, 9, 10, 0b0101011011 },
        { 6, 10, 10, 0b0101001010 },
        { 6, 11, 10, 0b0100110100 },
        { 6, 12, 10, 0b0100100011 },
        { 6, 13, 10, 0b0100010000 },
        { 6, 14, 11, 0b01000001000 },
        { 6, 15, 7, 0b0001010 },
        { 7, 0, 10, 0b0110110011 },
        { 7, 1, 8, 0b01110011 },
        { 7, 2, 8, 0b01101111 },
        { 7, 3, 8, 0b01101101 },
        { 7, 4, 9, 0b011010011 },
        { 7, 5, 9, 0b011001011 },
        { 7, 6, 9, 0b011000100 },
        { 7, 7, 9, 0b010111011 },
        { 7, 8, 10, 0b0101100001 },
        { 7, 9, 10, 0b0101001100 },
        { 7, 10, 10, 0b0100111001 },
        { 7, 11, 10, 0b0100101010 },
        { 7, 12, 10, 0b0100011011 },
        { 7, 13, 11, 0b01000010011 },
        { 7, 14, 11, 0b00101111101 },
        { 7, 15, 8, 0b00010001 },
        { 8, 0, 10, 0b0110101011 },
        { 8, 1, 9, 0b011010100 },
        { 8, 2, 9, 0b011010000 },
        { 8, 3, 9, 0b011001101 },
        { 8, 4, 9, 0b011001001 },
        { 8, 5, 9, 0b011000001 },
        { 8, 6, 9, 0b010111010 },
        { 8, 7, 9, 0b010110001 },
        { 8, 8, 9, 0b010101001 },
        { 8, 9, 10, 0b0101000000 },
        { 8, 10, 10, 0b0100101111 },
        { 8, 11, 10, 0b0100011110 },
        { 8, 12, 10, 0b0100001100 },
        { 8, 13, 11, 0b01000000010 },
        { 8, 14, 11, 0b00101111001 },
        { 8, 15, 8, 0b00010000 },
        { 9, 0, 10, 0b0101001111 },
        { 9, 1, 9, 0b011000111 },
        { 9, 2, 9, 0b011000101 },
        { 9, 3, 9, 0b010111111 },
        { 9, 4, 9, 0b010111101 },
        { 9, 5, 9, 0b010110101 },
        { 9, 6, 9, 0b010101110 },
        { 9, 7, 10, 0b0101001101 },
        { 9, 8, 10, 0b0101000001 },
        { 9, 9, 10, 0b0100110001 },
        { 9, 10, 10, 0b0100100001 },
        { 9, 11, 10, 0b0100010011 },
        { 9, 12, 11, 0b01000001001 },
        { 9, 13, 11, 0b00101111011 },
        { 9, 14, 11, 0b00101110011 },
        { 9, 15, 8, 0b00001011 },
        { 10, 0, 11, 0b01010011100 },
        { 10, 1, 9, 0b010111000 },
        { 10, 2, 9, 0b010110111 },
        { 10, 3, 9, 0b010110011 },
        { 10, 4, 9, 0b010101111 },
        { 10, 5, 10, 0b0101011000 },
        { 10, 6, 10, 0b0101001011 },
        { 10, 7, 10, 0b0100111010 },
        { 10, 8, 10, 0b0100110000 },
        { 10, 9, 10, 0b0100100010 },
        { 10, 10, 10, 0b0100010101 },
        { 10, 11, 11, 0b01000010010 },
        { 10, 12, 11, 0b00101111111 },
        { 10, 13, 11, 0b00101110101 },
        { 10, 14, 11, 0b00101101110 },
        { 10, 15, 8, 0b00001010 },
        { 11, 0, 11, 0b01010001100 },
        { 11, 1, 10, 0b0101011010 },
        { 11, 2, 9, 0b010101011 },
        { 11, 3, 9, 0b010101000 },
        { 11, 4, 9, 0b010100100 },
        { 11, 5, 10, 0b0100111110 },
        { 11, 6, 10, 0b0100110101 },
        { 11, 7, 10, 0b0100101011 },
        { 11, 8, 10, 0b0100011111 },
        { 11, 9, 10, 0b0100010100 },
        { 11, 10, 10, 0b0100000111 },
        { 11, 11, 11, 0b01000000001 },
        { 11, 12, 11, 0b00101110111 },
        { 11, 13, 11, 0b00101110000 },
        { 11, 14, 11, 0b00101101010 },
        { 11, 15, 8, 0b00000110 },
        { 12, 0, 11, 0b01010001000 },
        { 12, 1, 10, 0b0101000010 },
        { 12, 2, 10, 0b0100111100 },
        { 12, 3, 10, 0b0100111000 },
        { 12, 4, 10, 0b0100110011 },
        { 12, 5, 10, 0b0100101110 },
        { 12, 6, 10, 0b0100100100 },
        { 12, 7, 10, 0b0100011100 },
        { 12, 8, 10, 0b0100001101 },
        { 12, 9, 10, 0b0100000101 },
        { 12, 10, 11, 0b01000000000 },
        { 12, 11, 11, 0b00101111000 },
        { 12, 12, 11, 0b00101110010 },
        { 12, 13, 11, 0b00101101100 },
        { 12, 14, 11, 0b00101100111 },
        { 12, 15, 8, 0b00000100 },
        { 13, 0, 11, 0b01001101100 },
        { 13, 1, 10, 0b0100101100 },
        { 13, 2, 10, 0b0100101000 },
        { 13, 3, 10, 0b0100100110 },
        { 13, 4, 10, 0b0100100000 },
        { 13, 5, 10, 0b0100011010 },
        { 13, 6, 10, 0b0100010001 },
        { 13, 7, 10, 0b0100001010 },
        { 13, 8, 11, 0b01000000011 },
        { 13, 9, 11, 0b00101111100 },
        { 13, 10, 11, 0b00101110110 },
        { 13, 11, 11, 0b00101110001 },
        { 13, 12, 11, 0b00101101101 },
        { 13, 13, 11, 0b00101101001 },
        { 13, 14, 11, 0b00101100101 },
        { 13, 15, 8, 0b00000010 },
        { 14, 0, 12, 0b010000001001 },
        { 14, 1, 10, 0b0100011000 },
        { 14, 2, 10, 0b0100010110 },
        { 14, 3, 10, 0b0100010010 },
        { 14, 4, 10, 0b0100001011 },
        { 14, 5, 10, 0b0100001000 },
        { 14, 6, 10, 0b0100000011 },
        { 14, 7, 11, 0b00101111110 },
        { 14, 8, 11, 0b00101111010 },
        { 14, 9, 11, 0b00101110100 },
        { 14, 10, 11, 0b00101101111 },
        { 14, 11, 11, 0b00101101011 },
        { 14, 12, 11, 0b00101101000 },
        { 14, 13, 11, 0b00101100110 },
        { 14, 14, 11, 0b00101100100 },
        { 14, 15, 8, 0b00000000 },
        { 15, 0, 8, 0b00101011 },
        { 15, 1, 7, 0b0010100 },
        { 15, 2, 7, 0b0010011 },
        { 15, 3, 7, 0b0010001 },
        { 15, 4, 7, 0b0001111 },
        { 15, 5, 7, 0b0001101 },
        { 15, 6, 7, 0b0001011 },
        { 15, 7, 7, 0b0001001 },
        { 15, 8, 7, 0b0000111 },
        { 15, 9, 7, 0b0000110 },
        { 15, 10, 7, 0b0000100 },
        { 15, 11, 8, 0b00000111 },
        { 15, 12, 8, 0b00000101 },
        { 15, 13, 8, 0b00000011 },
        { 15, 14, 8, 0b00000001 },
        { 15, 15, 4, 0b0011 },
    } };

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
