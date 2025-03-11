/**
 * @file hashfunction.h
 * @author Krisna Pranav
 * @brief hash function
 * @version 6.0
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/stringview.h>
#include <mods/types.h>

namespace Crypto 
{

    namespace Hash 
    {
        /**
         * @tparam DigestS 
         */
        template<size_t DigestS>
        struct Digest {
            static_assert(DigestS % 8 == 0);
            constexpr static size_t Size = DigestS / 8;
            u8 data[Size];

            /**
             * @return ALWAYS_INLINE const* 
             */
            [[nodiscard]] ALWAYS_INLINE u8 const* immutable_data() const 
            { 
                return data; 
            }

            /**
             * @return ALWAYS_INLINE 
             */
            [[nodiscard]] ALWAYS_INLINE size_t data_length() const 
            { 
                return Size; 
            }

            /**
             * @return ALWAYS_INLINE 
             */
            [[nodiscard]] ALWAYS_INLINE ReadonlyBytes bytes() const 
            { 
                return { immutable_data(), data_length() }; 
            }
        }; // struct Digest

        /**
         * @tparam BlockS 
         * @tparam DigestS 
         * @tparam DigestT 
         */
        template<size_t BlockS, size_t DigestS, typename DigestT = Digest<DigestS>>
        class HashFunction 
        {
        public:
            static_assert(BlockS % 8 == 0);
            static constexpr auto BlockSize = BlockS / 8;

            static_assert(DigestS % 8 == 0);
            static constexpr auto DigestSize = DigestS / 8;

            using DigestType = DigestT;

            /**
             * @return constexpr size_t 
             */
            constexpr static size_t block_size() 
            { 
                return BlockSize; 
            }

            /**
             * @return constexpr size_t 
             */
            constexpr static size_t digest_size() 
            { 
                return DigestSize; 
            }

            virtual void update(u8 const*, size_t) = 0;

            /**
             * @param buffer 
             */
            void update(Bytes buffer) 
            { 
                update(buffer.data(), buffer.size()); 
            }

            /**
             * @param buffer 
             */
            void update(ReadonlyBytes buffer) 
            { 
                update(buffer.data(), buffer.size()); 
            }

            /**
             * @param buffer 
             */
            void update(ByteBuffer const& buffer) 
            { 
                update(buffer.data(), buffer.size()); 
            }

            /**
             * @param string 
             */
            void update(StringView string) 
            { 
                update((u8 const*)string.characters_without_null_termination(), string.length()); 
            }

            virtual DigestType peek() = 0;
            virtual DigestType digest() = 0;

            virtual void reset() = 0;

        #ifndef KERNEL
            virtual String class_name() const = 0;
        #endif

        protected:
            virtual ~HashFunction() = default;
        }; // class HashFunction

    } // namespace Hash

} // namespace Crypto
