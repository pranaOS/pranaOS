/**
 * @file hex.cpp
 * @author Krisna Pranav
 * @brief hex
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/array.h>
#include <mods/hex.h>
#include <mods/stringbuilder.h>
#include <mods/types.h>
#include <mods/vector.h>

namespace Mods
{
    /**
     * @param input 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> decode_hex(StringView input)
    {
        if((input.length() % 2) != 0)
            return Error::from_string_literal("Hex string was not an even length");

        auto output = TRY(ByteBuffer::create_zeroed(input.length() / 2));

        for(size_t i = 0; i < input.length() / 2; ++i)
        {
            auto const c1 = decode_hex_digit(input[i * 2]);
            if(c1 >= 16)
                return Error::from_string_literal("Hex string contains invalid digit");

            auto const c2 = decode_hex_digit(input[i * 2 + 1]);
            if(c2 >= 16)
                return Error::from_string_literal("Hex string contains invalid digit");

            output[i] = (c1 << 4) + c2;
        }

        return {move(output)};
    }

    #ifdef KERNEL
    /**
     * @param input 
     * @return ErrorOr<NonnullOwnPtr<Kernel::KString>> 
     */
    ErrorOr<NonnullOwnPtr<Kernel::KString>> encode_hex(const ReadonlyBytes input)
    {
        StringBuilder output(input.size() * 2);

        for(auto ch : input)
            TRY(output.try_appendff("{:02x}", ch));

        return Kernel::KString::try_create(output.string_view());
    }
    #else
    String encode_hex(const ReadonlyBytes input)
    {
        StringBuilder output(input.size() * 2);

        for(auto ch : input)
            output.appendff("{:02x}", ch);

        return output.build();
    }
    #endif

} // namespace Mods