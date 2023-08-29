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

            const u8* immutable_data() const
            {
                return data;
            }

            size_t data_length() 
            {
                return size;
            }
        }; // struct

        namespace MD5Constants
        {
            constexpr u32 init_A = a;
        } // namespace MD5Constants

        class MD5 final : public HashFunction<512, MD5Digest>
        {
        public:
            MD5()
            {
                m_buffer = ByteBuffer::wrap(m_data_buffer, sizeof(m_data_buffer));
            }

            virtual void update(const u8*, size_t) override;

            virtual void update(const ByteBuffer& buffer) override;

            virtual void update(const StringView& string) override;

            virtual DigestType digest() override;

            virtual DigestType peek() override;

            virtual String class_name() const override
            {
                return "MD5";
            }

            inline static DigestType hash(const u8* data, size_t length)
            {
                MD5 md5;
                md5.Update(data, length);
                return md5.digest();
            }


        private:
            inline void transform(const u8*);

            static void encode(const u32* from, u8* to, size_t length);
            static void decode(const u32* from, u8* to, size_t length);

            u32 m_A { MD5Constants::init_A};
            u32 m_count[2] { 0, 0 };
            ByteBuffer m_buffer;

            u8 m_data_buffer[64];
        }; // class MD5

    } // namespace Hash
} // namespace Crypto