/**
 * @file md5.h
 * @author Krisna Pranav
 * @brief md5
 * @version 6.0
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcrypto/hash/hashfunction.h>
#include <mods/string.h>
#include <mods/types.h>


namespace Crypto 
{
    namespace Hash 
    {

        struct MD5Digest 
        {
            constexpr static size_t Size = 16;
            u8 data[Size];

            /**
             * @return const u8* 
             */
            const u8* immutable_data() const 
            { 
                return data; 
            }

            /**
             * @return size_t 
             */
            size_t data_length() 
            { 
                return Size; 
            }
        }; // struct

        namespace MD5Constants 
        {

            constexpr u32 init_A = 0x67452301;
            constexpr u32 init_B = 0xefcdab89;
            constexpr u32 init_C = 0x98badcfe;
            constexpr u32 init_D = 0x10325476;
            constexpr u32 S11 = 7;
            constexpr u32 S12 = 12;
            constexpr u32 S13 = 17;
            constexpr u32 S14 = 22;
            constexpr u32 S21 = 5;
            constexpr u32 S22 = 9;
            constexpr u32 S23 = 14;
            constexpr u32 S24 = 20;
            constexpr u32 S31 = 4;
            constexpr u32 S32 = 11;
            constexpr u32 S33 = 16;
            constexpr u32 S34 = 23;
            constexpr u32 S41 = 6;
            constexpr u32 S42 = 10;
            constexpr u32 S43 = 15;
            constexpr u32 S44 = 21;

            constexpr u8 PADDING[] = 
            {
                0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0
            };

        } // namespace MD5Contstants

        class MD5 final : public HashFunction<512, MD5Digest> 
        {
        public:
            /// @brief Construct a new MD5 object
            MD5()
            {
                m_buffer = ByteBuffer::wrap(m_data_buffer, sizeof(m_data_buffer));
            }

            /// @brief: update
            virtual void update(const u8*, size_t) override;

            /**
             * @param buffer 
             */
            virtual void update(const ByteBuffer& buffer) override 
            { 
                update(buffer.data(), buffer.size()); 
            };

            /**
             * @param string 
             */
            virtual void update(const StringView& string) override 
            { 
                update((const u8*)string.characters_without_null_termination(), string.length()); 
            };

            /// @brief: digest + peek
            virtual DigestType digest() override;
            virtual DigestType peek() override;
            
            /**
             * @return String 
             */
            virtual String class_name() const override 
            { 
                return "MD5"; 
            }

            /**
             * @param data 
             * @param length 
             * @return DigestType 
             */
            inline static DigestType hash(const u8* data, size_t length)
            {
                MD5 md5;
                md5.update(data, length);
                return md5.digest();
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(const ByteBuffer& buffer) 
            { 
                return hash(buffer.data(), buffer.size()); 
            }

            /**
             * @param buffer 
             * @return DigestType 
             */
            inline static DigestType hash(const StringView& buffer) 
            { 
                return hash((const u8*)buffer.characters_without_null_termination(), buffer.length()); 
            }

            /// @brief: reset
            inline virtual void reset() override
            {
                m_A = MD5Constants::init_A;
                m_B = MD5Constants::init_B;
                m_C = MD5Constants::init_C;
                m_D = MD5Constants::init_D;

                m_count[0] = 0;
                m_count[1] = 0;

                __builtin_memset(m_data_buffer, 0, sizeof(m_data_buffer));
            }

        private:
            /// @brief: transform
            inline void transform(const u8*);

            /**
             * @param from 
             * @param to 
             * @param length 
             */
            static void encode(const u32* from, u8* to, size_t length);
            static void decode(const u8* from, u32* to, size_t length);

            /// @breif: constatns
            u32 m_A { MD5Constants::init_A }, m_B { MD5Constants::init_B }, m_C { MD5Constants::init_C }, m_D { MD5Constants::init_D };

            /// @breif: count
            u32 m_count[2] { 0, 0 };

            /// @brief: buffer + data buffer
            ByteBuffer m_buffer;
            u8 m_data_buffer[64];
        }; // class MD5
    } // namespace Hash
} // namespace Crypto