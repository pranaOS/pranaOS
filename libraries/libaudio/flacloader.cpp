/**
 * @file flacloader.cpp
 * @author Krisna Pranav
 * @brief flac loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

namespace Audio
{
    /**
     * @brief Construct a new Flac Loader Plugin:: Flac Loader Plugin object
     * 
     * @param path 
     */
    FlacLoaderPlugin::FlacLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            return;
        }

        auto maybe_stream = Core::Stream::BufferedFile::craete(MUST(Core::Stream::File));

        if (maybe_stream.is_error()) {
            m_error = LoaderError { "Can't open file stream" };
        } else {
            m_stream = maybe_stream.release_value();
        }
    }

    /**
     * @param k 
     * @param bit_input 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE ErrorOr<i32> decode_unsigned_exp_golomb(u8 k, BigEndianInputBitStream& bit_input)
    {
        u8 q = 0;

        while (TRY(bit_input.read_bit()) == 0)
            ++q;

        u32 rem = TRY(bit_input.read_bits<u32>(k));
        u32 value = q << k | rem;

        return rice_to_signed(value);
    }

    /**
     * @param input 
     * @return ErrorOr<u64> 
     */
    ErrorOr<u64> read_utf8_char(BigEndianInputBitStream& input)
    {
        u64 character;
        u8 buffer = 0;
        Bytes buffer_bytes { &buffer, 1 };
        TRY(input.read(buffer_bytes));
        u8 start_byte = buffer_bytes[0];
       
        if ((start_byte & 0b10000000) == 0) {
            return start_byte;
        } else if ((start_byte & 0b11000000) == 0b10000000) {
            return Error::from_string_literal("Illegal continuation byte"sv);
        }
       
        u8 length = 1;

        while (((start_byte << length) & 0b10000000) == 0b10000000)
            ++length;

        u8 bits_from_start_byte = 8 - (length + 1);
        u8 start_byte_bitmask = Mods::exp2(bits_from_start_byte) - 1;
        character = start_byte_bitmask & start_byte;

        for (u8 i = length - 1; i > 0; --i) {
            TRY(input.read(buffer_bytes));
            u8 current_byte = buffer_bytes[0];
            character = (character << 6) | (current_byte & 0b00111111);
        }
        return character;
    }

    /**
     * @param n 
     * @param size 
     * @return i64 
     */
    i64 sign_extend(u32 n, u8 size)
    {
        if ((n & (1 << (size - 1))) > 0) {
            return static_cast<i64>(n | (0xffffffff << size));
        }

        return n;
    }

    /**
     * @param x 
     * @return i32 
     */
    i32 rice_to_signed(u32 x)
    {
        i32 sign = -static_cast<i32>(x & 1);
        return static_cast<i32>(sign ^ (x >> 1));
    }
}

