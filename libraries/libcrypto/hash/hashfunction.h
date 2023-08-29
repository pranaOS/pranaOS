/**
 * @file hashfunction.h
 * @author Krisna Pranav
 * @brief hash function
 * @version 6.0
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/string_view.h>
#include <mods/types.h>

namespace Crypto 
{
    namespace Hash 
    {

        template<size_t BlockS, typename DigestT>
        class HashFunction 
        {
        public:
            static constexpr auto BlockSize = BlockS / 8;
            static constexpr auto DigestSize = DigestT::Size;

            using DigestType = DigestT;

            /**
             * @return size_t 
             */
            static size_t block_size() 
            { 
                return BlockSize; 
            };

            static size_t digest_size() 
            { 
                return DigestSize; 
            };

            /// @brief: update
            virtual void update(const u8*, size_t) = 0;

            /**
             * @param buffer 
             */
            virtual void update(const ByteBuffer& buffer) 
            { 
                update(buffer.data(), buffer.size()); 
            };

            /**
             * @param string 
             */
            virtual void update(const StringView& string) 
            { 
                update((const u8*)string.characters_without_null_termination(), string.length()); 
            };

            /// @brief: peek + digest
            virtual DigestType peek() = 0;
            virtual DigestType digest() = 0;

            /// @brief: reset
            virtual void reset() = 0;

            /**
             * @return String 
             */
            virtual String class_name() const = 0;
        }; // class HashFunction
    } // namespace Hash
} // namespace Crypto
