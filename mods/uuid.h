/**
 * @file uuid.h
 * @author Krisna Pranav
 * @brief UUID
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

namespace Mods 
{
    class UUID
    {
    public:
        enum class Endianness
        {
            Mixed,
            Little
        }; // enum class Endianness

        /**
         * @brief Construct a new UUID object
         * 
         */
        UUID() = default;
    
    private:
        void convert_string_view_to_little_endian_uuid(StringView);

        void convert_string_view_to_mixed_endian_uuid(StringView);

        Array<u8, 16> m_uuid_buffer {};
    }; // class UUID
}