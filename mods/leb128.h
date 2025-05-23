/**
 * @file leb128.h
 * @author Krisna Pranav
 * @brief LEB 128
 * @version 6.0
 * @date 2024-11-10
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/numericlimits.h>
#include <mods/stream.h>
#include <mods/types.h>

namespace Mods
{
    struct LEB128
    {
        /**
         * @tparam StreamT 
         * @tparam ValueType 
         * @param stream 
         * @param result 
         * @return true 
         * @return false 
         */
        template <typename StreamT, typename ValueType = size_t>
        static bool read_unsigned(StreamT& stream, ValueType& result)
        {
            [[maybe_unused]] size_t backup_offset = 0;
            
            if constexpr(requires { stream.offset(); })
                backup_offset = stream.offset();

            InputStream& input_stream{stream};

            result = 0;
            size_t num_bytes = 0;
            while(true)
            {
                if(input_stream.unreliable_eof())
                {
                    if constexpr(requires { stream.seek(backup_offset); })
                        stream.seek(backup_offset);
                    input_stream.set_fatal_error();
                    return false;
                }
                u8 byte = 0;
                input_stream >> byte;

                if(input_stream.has_any_error())
                    return false;

                ValueType masked_byte = byte & ~(1 << 7);
                bool const shift_too_large_for_result = (num_bytes * 7 > sizeof(ValueType) * 8) && (masked_byte != 0);
                if(shift_too_large_for_result)
                    return false;

                bool const shift_too_large_for_byte = ((masked_byte << (num_bytes * 7)) >> (num_bytes * 7)) != masked_byte;
                if(shift_too_large_for_byte)
                    return false;

                result = (result) | (masked_byte << (num_bytes * 7));
                if(!(byte & (1 << 7)))
                    break;
                ++num_bytes;
            }

            return true;
        }

        /**
         * @tparam StreamT 
         * @tparam ValueType 
         * @param stream 
         * @param result 
         * @return true 
         * @return false 
         */
        template <typename StreamT, typename ValueType = ssize_t>
        static bool read_signed(StreamT& stream, ValueType& result)
        {
            static_assert(sizeof(ValueType) <= sizeof(u64), "Error checking logic assumes 64 bits or less!");
            [[maybe_unused]] size_t backup_offset = 0;

            if constexpr(requires { stream.offset(); })
                backup_offset = stream.offset();

            InputStream& input_stream{stream};

            i64 temp = 0;
            size_t num_bytes = 0;
            u8 byte = 0;
            result = 0;

            do
            {
                if(input_stream.unreliable_eof())
                {
                    if constexpr(requires { stream.seek(backup_offset); })
                        stream.seek(backup_offset);
                    input_stream.set_fatal_error();
                    return false;
                }

                input_stream >> byte;
                if(input_stream.has_any_error())
                    return false;

                u64 masked_byte = byte & ~(1 << 7);

                bool const shift_too_large_for_result = (num_bytes * 7 >= 64) && (masked_byte != ((temp < 0) ? 0x7Fu : 0u));

                if(shift_too_large_for_result)
                    return false;

                bool const shift_too_large_for_byte = (num_bytes * 7) == 63 && masked_byte != 0x00 && masked_byte != 0x7Fu;
                if(shift_too_large_for_byte)
                    return false;

                temp = (temp) | (masked_byte << (num_bytes * 7));
                ++num_bytes;
            } while(byte & (1 << 7));

            if((num_bytes * 7) < 64 && (byte & 0x40))
            {
                temp |= ((u64)(-1) << (num_bytes * 7));
            }

            if constexpr(sizeof(ValueType) < sizeof(u64))
            {
                if(temp > NumericLimits<ValueType>::max() || temp < NumericLimits<ValueType>::min())
                    return false;
            }

            result = static_cast<ValueType>(temp);

            return true;
        }
    }; // struct LEB128
} // namespace Mods

using Mods::LEB128;
