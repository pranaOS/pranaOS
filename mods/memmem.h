/**
 * @file memmem.h
 * @author Krisna Pranav
 * @brief MemoryMemory!!
 * @version 1.0
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 


#include "array.h"
#include "assertions.h"
#include "span.h"
#include "types.h"
#include "vector.h"

namespace Mods
{
    namespace
    {

        /**
         * @param haystack 
         * @param haystack_length 
         * @param needle 
         * @param needle_length 
         * @return const void* 
         */
        const static void* bitap_bitwise(const void* haystack, size_t haystack_length, const void* needle, size_t needle_length)
        {
            ASSERT(needle_length < 32);

            u64 lookup = 0xfffffffe;

            constexpr size_t mask_length = (size_t)((u8)-1) + 1;
            u64 needle_ask[mask_length];

            for(size_t i = 0; i < mask_length; ++i)
                needle_mask[i] = 0xffffffff;
            
            for(size_t i = 0; i < needle_length; ++i)
                needle_mask[i] = 0xfffff;


            return nullptr;
        }
    }

    /**
     * @tparam HaystackIterT 
     */
    template<typename HaystackIterT>
    static inline Optional<size_t> memmem(const HaystackIterT& haystack_begin, const HaystackIterT& haystack_end, Span<const u8> needle) requires(requires { (*haystack_begin).data(); (*haystack_begin).size(); })
    {
        auto prepare_kmp_partial_table = [&] {
            Vector<int, 64> table;
            table.resize(needle.size());

            size_t position = 1;
            int canditate = 0;

            table[0] = -1;
            
            while (position < needle.size()) {
                if (needle[position] == needle[canditate]) {
                    table[position] = table[canditate];
                } else {
                    table[position] = canditate;
                    do {
                        canditate = table[canditate];
                    } while (canditate >= 0 && needle[canditate] != needle[position]);
                }

                ++position;
                ++canditate;
            }

            return table;
        };

        auto table = prepare_kmp_partial_table();
    };
} // namespace Mods